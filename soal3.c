#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <dirent.h> 
#include <stdio.h>
#include <string.h>

int main(){
	pid_t pid = fork();

	if(pid < 0)
		exit(EXIT_FAILURE);

	if(chdir("/home/geizka") < 0){
		exit(EXIT_FAILURE);
	}

	if(pid == 0){
		pid = fork();
		if(pid < 0)
			exit(EXIT_FAILURE);
		if(pid == 0){
			char * arg[] = {"mkdir", "-p", "./modul2/indomie", NULL};
			execv("/bin/mkdir", arg);
		}
		wait(NULL);
		sleep(5);
		char * arg[] = {"mkdir", "-p", "./modul2/sedaap", NULL};
		execv("/bin/mkdir", arg);
	}
	wait(NULL);
	pid = fork();
	if(pid < 0)
		exit(EXIT_FAILURE);
	if(pid == 0){
		char * arg[] = {"unzip", "./modul2/jpg.zip", "-d", "./modul2", NULL};
		execv("/usr/bin/unzip", arg);
	}
	wait(NULL);
	struct dirent * de;
	DIR * directory = opendir("./modul2/jpg");
	while((de = readdir(directory)) != NULL){
		if(strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
			continue;
		char fileName[300];
		char destName[300];
		if(de->d_type == DT_DIR){
			snprintf(fileName, sizeof(fileName), "./modul2/jpg/%s", de->d_name);
			snprintf(destName, sizeof(destName), "./modul2/indomie/%s", de->d_name);
			pid = fork();
			if(pid == 0){
				char * arg[] = {"mv", fileName, destName, NULL};
				execv("/bin/mv", arg);
			}
			wait(NULL);
			snprintf(fileName, sizeof(fileName), "%s/coba1.txt", destName);
			pid = fork();
			if(pid == 0){
				char * arg[] = {"touch", fileName, NULL};
				execv("/usr/bin/touch", arg);
			}
			wait(NULL);
			sleep(3);
			snprintf(fileName, sizeof(fileName), "%s/coba2.txt", destName);
			pid = fork();
			if(pid == 0){
				char * arg[] = {"touch", fileName, NULL};
				execv("/usr/bin/touch", arg);
			}
			wait(NULL);
		}else{
			snprintf(fileName, sizeof(fileName), "./modul2/jpg/%s", de->d_name);
			snprintf(destName, sizeof(destName), "./modul2/sedaap/%s", de->d_name);
			pid = fork();
			if(pid == 0){
				char * arg[] = {"mv", fileName, destName, NULL};
				execv("/bin/mv", arg);
			}
			wait(NULL);
		}
	}
	return 0;
}