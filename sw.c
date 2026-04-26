#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


int main(int argc, char *argv[]){
	if(argc == 1){
															// initializing the variables
		char *monitor; 												// used to store the name of the monitor were currently searching the wallpaper for
		char path[512]; 											// used to store the path to the monitorfiles
		char betweenpath[256]; 											// used to store the path to the wallpapers
		snprintf(betweenpath, sizeof(betweenpath), "%s/Documents/wallpapers/", getenv("HOME"));
		char fehcommand[512]; 											//used to store the command which will show the wallpapers
		snprintf(fehcommand, sizeof(fehcommand), "feh --bg-scale");
		char buffer[256]; 											// used as a buffer
		char moninfo[256]; 											// used to store the output of the getmonitors command
		char picture[256]; 											// used as a buffer
		char wallpapers[256];

		FILE *getmonitors = popen("xrandr --listmonitors", "r"); 				// get the currently active monitors
		if(!getmonitors){
			printf("ERROR: not able to execute xrandr --listmonitors\n");
			return 1;
		}
		while(fgets(moninfo, sizeof(moninfo), getmonitors)){ 				// do the following for every active monitor

			moninfo[strcspn(moninfo, "\n")] = 0;	

			monitor = strtok(moninfo, " ");
			if(strcmp(monitor, "Monitors:")==0){
				continue;
			}
			monitor = strtok(NULL, " ");
			monitor = strtok(NULL, " ");
			monitor = strtok(NULL, " ");
			snprintf(path, sizeof(path), "%s/.config/cw/%s", getenv("HOME"), monitor);

			FILE *getpicture = fopen(path, "r"); 						//get the currently set picture index from the monitorfiles (0 if no file)
			if(!getpicture){
				DIR *direct;
				struct dirent *dir;
				direct = opendir(betweenpath);
				if(direct){
					while((dir = readdir(direct))!=0){
						if(dir->d_type != DT_REG || strcmp(".", dir->d_name) == 0 || strcmp(".", dir->d_name) == 0){
							continue;
						}
						snprintf(picture, sizeof(picture), dir->d_name);
						break;
					}	
					closedir(direct);
					getpicture = fopen(path, "w+");
					if(!getpicture){
						printf("Not able to create %s.", path);
						return 1;
					}
					fputs(picture, getpicture);
					fclose(getpicture);
				}else{
					printf("Not found the wallpapersdirectory.\n");
					return 1;
				}
			}else{
				while(fgets(picture, sizeof(picture), getpicture)){
					continue;
				}
				fclose(getpicture);
			}

			picture[strcspn(picture, "\n")] = 0;	
			strncat(fehcommand, " ", sizeof(fehcommand) - strlen(fehcommand) - 1);
			strncat(fehcommand, betweenpath, sizeof(fehcommand) - strlen(fehcommand) - 1);
			strncat(fehcommand, picture, sizeof(fehcommand) - strlen(fehcommand) - 1);

		}
		pclose(getmonitors);

		strncat(fehcommand," --no-fehbg" , sizeof(fehcommand) - strlen(fehcommand) - 1);

		system(fehcommand); 		// execute the command that shows the wallpapers

	}else{
		printf("No argument expected\n");
		return 1;
	}

	return 0;
}
