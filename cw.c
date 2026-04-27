#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include "getmonitor.h"


int main(int argc, char *argv[]){
	if(argc == 1){
						// Initialization of Variables	
		int count = 0; 			// used to count through the images in the wallpapers folder
		char picture[256]; 		// used to store the value which is/will be written into the monitor file
		bool created = false; 		// used to store whether the file of the monitor has been created yet or not
		char *monitor; 			// used to get the name of the monitor
		char path[512]; 		// used to store the path to the monitorfile
		char wallpaperspath[256]; 	// used to store thee command to get the amount of the wallpapers
		char firstpicture[256]; 	// stores the first picture
		bool next = false;
		bool first = true;

		monitor = getmonitor(); 							// get the current monitor (see getmonitor.c)
		snprintf(path, sizeof(path), "%s/.config/cw/%s", getenv("HOME"), monitor); 	// path is the path to its file
		free(monitor);

		FILE *getcurrentwp = fopen(path, "r"); 		// getcurrentwallpaper from monitorfile
		if(!getcurrentwp){
			created = true;
		}else{
			while(fgets(picture, sizeof(picture), getcurrentwp) != NULL){
				continue;
			}
			fclose(getcurrentwp);
		}

		snprintf(wallpaperspath, sizeof(wallpaperspath), "%s/Documents/wallpapers/", getenv("HOME"));

		DIR *direct;
		struct dirent *dir;
		direct = opendir(wallpaperspath);
		if(direct){
			while((dir=readdir(direct))!=0){
				if(strcmp(dir->d_name, ".") == 0 ||strcmp(dir->d_name, "..") == 0 || dir->d_type != DT_REG){
					continue;
				}
				if(first){
					first = false;
					snprintf(firstpicture, sizeof(firstpicture), dir->d_name);
					if(created){
						break;
					}
					created = true;
				}
				if(next){
					snprintf(picture, sizeof(picture),dir->d_name);
					next = false;
					break;
				}
				if(strcmp(dir->d_name, picture) == 0){
					next = true;
					created = false;
				}
			}
			closedir(direct);
			if(next || created){
				snprintf(picture, sizeof(picture),firstpicture);
			}
		}else{
			fprintf(stderr, "Not able to get files in %s.\n", wallpaperspath);
			return 1;
		}

		FILE *writenewpicture = fopen(path, "w+"); 		// writes the name of the picture to the file
		if(!writenewpicture){
			fprintf(stderr, "Cannot open/create %s.\n", path);
			return 1;
		}
		fputs(picture, writenewpicture);
		fclose(writenewpicture);
		
		system("sw"); 					 //update the currently shown wallpapers from the monitor files



	}else{
		fprintf(stderr, "Too many arguments given\n");
		return 1;
	}

	return 0;
}
