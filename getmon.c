#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "getmonitor.h"


int main(int argc, char *argv[]){
	if(argc == 1){
		char *monitor = getmonitor();
		if(monitor == NULL){
			return 1;
		}
		printf("%s\n", monitor);
		free(monitor);
	}else{
		printf("No argument expected\n");
		return 1;
	}
	return 0;
}
