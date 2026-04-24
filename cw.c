#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "getmonitor.h"


int main(int argc, char *argv[]){
	if(argc == 1){
						// Initialization of Variables	
		int count = 0; 			// used to count through the images in the wallpapers folder
		int picture = 0; 		// used to store the value which is/will be written into the monitor file
		bool created = false; 		// used to store whether the file of the monitor has been created yet or not
		char *monitor; 			// used to get the name of the monitor
		char path[512]; 		// used to store the path to the monitorfile
		char wallpaperspath[256]; 	// used to store thee command to get the amount of the wallpapers
		char buffer[256]; 		// used as a buffer
		char *end; 			// used because one function needs to return something even thopugh i dont need it

		monitor = getmonitor(); 							// get the current monitor (see getmonitor.c)
		snprintf(path, sizeof(path), "%s/.config/cw/%s", getenv("HOME"), monitor); 	// path is the path to its file
		free(monitor);

		FILE *getcurrentwp = fopen(path, "r"); 		// getcurrentwallpaper from monitorfile
		if(!getcurrentwp){
			created = true;
		}else{
			while(fgets(buffer, sizeof(buffer), getcurrentwp) != NULL){
				picture = strtol(buffer, &end, 10);
			}
			fclose(getcurrentwp);
		}

		snprintf(wallpaperspath, sizeof(wallpaperspath), "ls %s/Documents/wallpapers/", getenv("HOME"));

		FILE *countwps = popen(wallpaperspath, "r");  				// counts the wallpapers in the wallpaperdirectory
		if(!countwps){
			printf("Not able to execute %s.\n", wallpaperspath);
			return 1;
		}
		if(created){   					// if there hasnt been a file created yet take the pictur with index 0
			picture = 0;
		}else{
			while(fgets(buffer, sizeof(buffer), countwps) != NULL){
				count += 1;
			}
			if(picture + 1 >= count){ 		// sets the picture to be written to the monitor file to the last index +1 (0 if it is bigger than the amount of wallpapers
				picture = 0;
			}else{
				picture += 1;
			}
		}
		pclose(countwps);

		FILE *writenewpicture = fopen(path, "w+"); 		// writes the picturevalue to the file
		if(!writenewpicture){
			printf("Cannot open %s.\n", path);
			return 1;
		}
		sprintf(buffer, "%d", picture);
		fputs(buffer, writenewpicture);
		fclose(writenewpicture);
		
		system("sw"); 					// update the currently shown wallpapers from the monitor files



	}else{
		printf("Too many arguments given\n");
		return 1;
	}

	return 0;
}
