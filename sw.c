#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <stdbool.h>


int main(int argc, char *argv[]){
	if(argc == 1){
															// initializing the variables
		int monitor_count = 0;
		char path[512]; 											// used to store the path to the monitorfiles
		char betweenpath[256]; 											// used to store the path to the wallpapers
		snprintf(betweenpath, sizeof(betweenpath), "%s/Documents/wallpapers/", getenv("HOME"));
		char fehcommand[512]; 											//used to store the command which will show the wallpapers
		snprintf(fehcommand, sizeof(fehcommand), "feh --bg-scale");
		char picture[256]; 											// stores name of the picture

		Display *display = XOpenDisplay(NULL);
		if(!display){
			fprintf(stderr, "Not able to open display!\n");
			return 1;
		}
		Window root = DefaultRootWindow(display);
		XRRScreenResources *screenResources = XRRGetScreenResources(display, root);
		if(!screenResources){
			fprintf(stderr, "Unable to get screen resources!\n");
			XCloseDisplay(display);
			return 1;
		}

		for(int i = 0; i< screenResources->noutput; ++i){
			XRROutputInfo *outputInfo=XRRGetOutputInfo(display, screenResources,screenResources->outputs[i]);
			if(outputInfo->connection == RR_Connected){
				snprintf(path, sizeof(path), "%s/.config/cw/%s", getenv("HOME"), outputInfo->name);

				FILE *getpicture = fopen(path, "r"); 						//get the currently set picture for the monitor
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
							fprintf(stderr, "Not able to create %s.", path);
							return 1;
						}
						fputs(picture, getpicture);
						fclose(getpicture);
					}else{
						fprintf(stderr, "Not found the wallpapersdirectory.\n");
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

			XRRFreeOutputInfo(outputInfo);
		}

		strncat(fehcommand," --no-fehbg" , sizeof(fehcommand) - strlen(fehcommand) - 1);

		system(fehcommand); 		// execute the command that shows the wallpapers

		XRRFreeScreenResources(screenResources);
		XCloseDisplay(display);

	}else{
		fprintf(stderr, "No argument expected\n");
		return 1;
	}

	return 0;
}
