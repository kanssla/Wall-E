#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


int main(int argc, char *argv[]){
	if(argc == 1){
															// initializing the variables
		char *monitor; 												// used to store the name of the monitor were currently searching the wallpaper for
		char path[512]; 											// used to store the path to the monitorfiles
		char wallpaperspath[256]; 										// used to store the command to get the names of the wallpapers
		snprintf(wallpaperspath, sizeof(wallpaperspath), "ls %s/Documents/wallpapers/", getenv("HOME"));
		char betweenpath[256]; 											// used to store the path to the wallpapers
		snprintf(betweenpath, sizeof(betweenpath), "%s/Documents/wallpapers/", getenv("HOME"));
		char fehcommand[512]; 											//used to store the command which will show the wallpapers
		snprintf(fehcommand, sizeof(fehcommand), "feh --bg-scale");
		char buffer[256]; 											// used as a buffer
		char moninfo[256]; 											// used to store the output of the getmonitors command
		char *end; 												// used because one function needs it but i dont
		int picture = 0; 											// used to store the index of the picture which will be used
		int count = 0; 												// used  to count through the wallpapers in the directory
		

		FILE *getmonitors = popen("xrandr --listmonitors", "r"); 				// get the currently active monitors
		if(!getmonitors){
			printf("ERROR: not able to execute xrandr --listmonitors\n");
			return 1;
		}
		while(fgets(moninfo, sizeof(moninfo), getmonitors)){ 				// do the following for every active monitor

			count = 0;
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
				printf("Not found a Wallpaper for %s.\n", monitor);
				picture = 0;
			}else{
				while(fgets(buffer, sizeof(buffer), getpicture)){
					picture = strtol(buffer, &end, 10);
				}
				fclose(getpicture);
			}

			FILE *getfilenames = popen(wallpaperspath, "r"); 				// convert the index into the name of the wallpaper and add it to the current fehcommand
			if(!getfilenames){
				printf("Not able to find the wallpapers.\n");
				return 1;
			}
			while(fgets(buffer, sizeof(buffer), getfilenames)){
				if(count == picture){
					buffer[strcspn(buffer, "\n")] = 0;	
					strncat(fehcommand, " ", sizeof(fehcommand) - strlen(fehcommand) - 1);
					strncat(fehcommand, betweenpath, sizeof(fehcommand) - strlen(fehcommand) - 1);
					strncat(fehcommand, buffer, sizeof(fehcommand) - strlen(fehcommand) - 1);
					break;
				}
				count += 1;
			}
			pclose(getfilenames);
			
		}
		pclose(getmonitors);

		snprintf(path, sizeof(path), "%s --no-fehbg", fehcommand); 		// dont make a .fehbg file (see feh docs); also theres a misuse of the path vareiable because its comfortable and works (stuff that belongs to fehcommand is stored in path, but we dont use path anymore)

		system(path); 		// execute the command that shows the wallpapers

	}else{
		printf("No argument expected\n");
		return 1;
	}

	return 0;
}
