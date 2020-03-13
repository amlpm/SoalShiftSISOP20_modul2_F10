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

int isStar(char * str){
	int length = strlen(str);
	if(length == 1){
		if(strcmp(str, "*") == 0)return 1;
	}
	return 0;
}

int isNumber(char * str){
	int length = strlen(str);
	for(int i = 0; i < length; i++){
		if(!isdigit(str[i]))
			return 0;
	}
}

void printError(){
	printf("Invalid Parameter\n");
}

void assignParam(int * num, int min, int max, char * str){
	if(isNumber(str)){
		*num = atoi(str);
		if(*num < min || *num > max){
			printError();
			exit(EXIT_FAILURE);
		}
	}else if(isStar(str)){
		*num = -1;
	}else{
		printError();
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char ** argv) {

	if(argc != 5){
		printError();
		exit(EXIT_FAILURE);
	}

	char * basharg[] = {"bash", argv[4], NULL};

	int second, minute, hour;
	
	assignParam(&second, 0, 59, argv[1]);
	assignParam(&minute, 0, 59, argv[2]);
	assignParam(&hour, 0, 23, argv[3]);

	if ((chdir("/")) < 0) {
		exit(EXIT_FAILURE);
	}

	FILE * file = fopen(argv[4], "r");
	if(!file){
		printf("Invalid Path\n");
		exit(EXIT_FAILURE);
	}
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
		time_t t = time(NULL);
		struct tm * timeNow = localtime(&t);
		int timeToSleep = 0;
		if(hour == -1){

		}else if(timeNow->tm_hour <= hour){
			timeToSleep += hour - timeNow->tm_hour;
		}else{
			timeToSleep += 24 - timeNow->tm_hour + hour;
		}
		timeToSleep *= 60;
		if(minute == -1){

		}else if(timeNow->tm_min <= minute){
			timeToSleep += minute - timeNow->tm_min;
		}else{
			if(timeToSleep > 0)
				timeToSleep -= 60;
			timeToSleep += 60 - timeNow->tm_min + minute;
		}
		timeToSleep *= 60;
		if(second == -1){
			
		}else if(timeNow->tm_sec <= second){
			timeToSleep += second - timeNow->tm_sec;
		}else{
			if(timeToSleep > 0)
				timeToSleep -= 60;
			timeToSleep += 60 - timeNow->tm_sec + second;
		}
		if(timeToSleep == 0)timeToSleep = 1;
		sleep(timeToSleep);
		pid_t pid = fork();
		if(pid < 0)
			exit(EXIT_FAILURE);
		if(pid == 0){
			char dir[100];
			strcpy(dir, argv[4]);
			int dirEndIndex = 0;
			for(int i = strlen(dir); i >= 0; i--){
				if(dir[i] == '/'){
					dirEndIndex = i;
					break;
				}
			}
			dir[dirEndIndex] = '\0';
			chdir(dir);
			execv("/bin/bash", basharg);
		}

	}
}