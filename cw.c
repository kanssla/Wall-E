#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "getmonitor.h"


int main(int argc, char *argv[]){
	if(argc == 1){
		
		int count = 0;
		int picture = 0;
		bool created = false;
		char *monitor;
		char path[512];
		char wallpaperspath[256];
		char buffer[256];
		char *end;

		monitor = getmonitor();
		snprintf(path, sizeof(path), "%s/.config/cw/%s", getenv("HOME"), monitor);
		free(monitor);

		FILE *getcurrentwp = fopen(path, "r");
		if(!getcurrentwp){
			created = true;
		}else{
			while(fgets(buffer, sizeof(buffer), getcurrentwp) != NULL){
				picture = strtol(buffer, &end, 10);
			}
			fclose(getcurrentwp);
		}

		snprintf(wallpaperspath, sizeof(wallpaperspath), "ls %s/Documents/wallpapers/", getenv("HOME"));

		FILE *countwps = popen(wallpaperspath, "r");
		if(!countwps){
			printf("Not able to execute %s.\n", wallpaperspath);
			return 1;
		}
		if(created){
			picture = 0;
		}else{
			while(fgets(buffer, sizeof(buffer), countwps) != NULL){
				count += 1;
			}
			if(picture + 1 >= count){
				picture = 0;
			}else{
				picture += 1;
			}
		}
		pclose(countwps);

		FILE *writenewpicture = fopen(path, "w+");
		if(!writenewpicture){
			printf("Cannot open %s.\n", path);
			return 1;
		}
		sprintf(buffer, "%d", picture);
		fputs(buffer, writenewpicture);
		fclose(writenewpicture);
		
		system("sw");



	}else{
		printf("Too many arguments given\n");
		return 1;
	}

	return 0;
}
