#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "getmonitor.h"



int inmonitor(int *mouse, int *bl, int *tr){
	if(mouse[0] >= bl[0] && tr[0] >= mouse[0] && mouse[1] >= bl[1] && tr[1] >= mouse[1]){
		return 1;
	}
	return 0;
}

char* getmonitor(){

	char buffer[256];
	char name[256];
	char *end;
	char *point;
	char *dimensions;
	int count;
	int mouse[2];
	int bl[2];
	int tr[2];
	FILE *command = popen("xdotool getmouselocation --shell","r");	
	if(!command){
		printf("Not able to get mouselocation. Please make sure xdotool is correctly installed.");
		return NULL;
	}
	while(fgets(buffer, sizeof(buffer), command)){
		memmove(buffer, buffer + 2, strlen(buffer)-1);
		if(count == 1){
			mouse[1] = strtol(buffer, &end, 10);
			break;
		}else{
			mouse[0] = strtol(buffer, &end, 10);
			count = 1;
		}
	}
	pclose(command);

	command = popen("xrandr", "r");
	if(!command){
		printf("Not able to get monitor information. Please make sure xrandr is correctly installed.");
		return NULL;
	}
	while(fgets(buffer, sizeof(buffer), command)){
		if(buffer[0] == ' '){
			continue;
		}
		point = strtok(buffer, " ");
		snprintf(name, sizeof(name), point);
		if(strcmp(name, "Screen") == 0){
			continue;
		}
		point = strtok(NULL, " ");
		if(strcmp(point, "disconnected") == 0){
			continue;
		}
		point = strtok(NULL, " ");
		if(strcmp(point, "primary") == 0){
			point = strtok(NULL, " ");
		}
		dimensions = strtok(point, "x+");
		tr[0] = strtol(dimensions, &end, 10);
		dimensions = strtok(NULL, "x+");
		tr[1] = strtol(dimensions, &end, 10);
		dimensions = strtok(NULL, "x+");
		bl[0] = strtol(dimensions, &end, 10);
		tr[0] += bl[0];
		dimensions = strtok(NULL, "x+");
		bl[1] = strtol(dimensions, &end, 10);
		tr[1] += bl[1];

		if(inmonitor(mouse, bl, tr)){
			size_t len = snprintf(NULL, 0,"%s", name)+1;
			char* str = malloc(len);
			if(str) snprintf(str, len, "%s", name);
			return str;
		}
	}
	printf("Mouse not to be found.\n");
	return NULL;
}
