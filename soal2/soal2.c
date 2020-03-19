#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <wait.h>

int main(int argc, char ** argv) {

	FILE * killer = fopen("killer.sh", "w");
	fprintf(killer, "#!/bin/bash\n");
	if(strcmp("-a", argv[1]) == 0){
		fprintf(killer, "kill $(pidof soal2)\n");
	}else if(strcmp("-b", argv[1]) == 0){
		fprintf(killer, "kill_parent(){\n");
		fprintf(killer, "kill ${@: -1}\n");
		fprintf(killer, "}\n");
		fprintf(killer, "kill_parent $(pidof soal2)\n");
	}
	fprintf(killer, "rm $0\n");
	fclose(killer);

	chmod("killer.sh", ~0);

	pid_t pid, sid;

	pid = fork();

 	if (pid < 0) {
		exit(EXIT_FAILURE);
	}
	
	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}

	umask(0);

	sid = setsid();
	if (sid < 0) {
		exit(EXIT_FAILURE);
	}

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	
	while (1) {
		pid_t p = fork();
		if(p == 0){
			time_t t = time(NULL);
			struct tm tt = *localtime(&t);
			char folderName[50];
			snprintf(folderName, sizeof(folderName), "%04d-%02d-%02d_%02d:%02d:%02d", tt.tm_year + 1900, tt.tm_mon+1, tt.tm_mday, tt.tm_hour, tt.tm_min, tt.tm_sec);
			if(p == 0){
				p = fork();
				if(p == 0){
					char * arg[] = {"mkdir", "-p", folderName, NULL};
					execv("/bin/mkdir", arg);
				}
				wait(NULL);
				for(int i = 0; i < 20; i++){
					p = fork();
					if(p == 0){
						t = time(NULL);
						tt = *localtime(&t);
						char name[50];
						snprintf(name, sizeof(name), "./%s/%04d-%02d-%02d_%02d:%02d:%02d", folderName, tt.tm_year + 1900, tt.tm_mon+1, tt.tm_mday, tt.tm_hour, tt.tm_min, tt.tm_sec);
						char url[25];
						snprintf(url, sizeof(url), "https://picsum.photos/%ld", t%1000+100);
						char * arg[] = {"wget", "-O", name, url, NULL};
						execv("/usr/bin/wget", arg);
					}
					sleep(5);
				}
				wait(NULL);
				char fileName[30];
				snprintf(fileName, sizeof(fileName), "%s.zip", folderName);
				char * arg[] = {"zip", "-rm", fileName, folderName, NULL};
				execv("/usr/bin/zip", arg);
			}
			exit(EXIT_SUCCESS);
		}
		sleep(30);
	}
}