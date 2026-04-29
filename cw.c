#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include "getmonitor.h"
#include "config.h"


int main(int argc, char *argv[]){

	char picture[256]; 		// used to store the value which is/will be written into the monitor file
	bool created = false; 		// used to store whether the file of the monitor has been created yet or not
	char path[512]; 		// used to store the path to the monitorfile
	char firstpicture[256]; 	// stores the first picture
	bool next = false;
	bool first = true;

	int count = 1;
	bool change = false;
	bool getmon = true;
	bool getwallpaper = true;
	bool set = true;

	if(count < argc){
		if(strcmp(argv[count], "change")==0){
			change = true;
			count++;
		}
	}
	if(count<argc){
		if(strcmp(argv[count], "-m")==0 && change){
			getmon = false;
			count++;
		
			if(count >= argc){
				fprintf(stderr,"No monitor provided.\n");
				return 1;
			}else{
				snprintf(path, sizeof(path), "%s%s", configpath, argv[count]); 	
				count++;
			}
		}
	}
	if(count<argc){
		if(strcmp(argv[count], "-p")==0 && change){
			getwallpaper = false;
			count++;

			if(count >= argc){
				fprintf(stderr,"No wallpaper provided.\n");
				return 1;
			}else{
				snprintf(picture, sizeof(picture), argv[count]); 
				count++;
			}
		}
	}
	if(count<argc){
		if(strcmp(argv[count], "noset")==0){
			set = false;
			count++;
		}
	}
	if(count<argc){
		fprintf(stderr, "False compilation of arguments.\n");
		return 1;
	}



	if(getmon){

		char *monitor = getmonitor(); 					// get the current monitor (see getmonitor.c)
		snprintf(path, sizeof(path), "%s%s", configpath, monitor); 	// path is the path to its file
		free(monitor);
	}
	if(getwallpaper){

		FILE *getcurrentwp = fopen(path, "r"); 		// getcurrentwallpaper from monitorfile
		if(!getcurrentwp){
			created = true;
		}else{
			while(fgets(picture, sizeof(picture), getcurrentwp) != NULL){
				continue;
			}
			fclose(getcurrentwp);
		}

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
	}
	if(change){

		FILE *writenewpicture = fopen(path, "w+"); 		// writes the name of the picture to the file
		if(!writenewpicture){
			fprintf(stderr, "Cannot open/create %s.\n", path);
			return 1;
		}
		fputs(picture, writenewpicture);
		fclose(writenewpicture);
	}
	if(set){
		int monitor_count = 0;
                char fehcommand[512];                                                                                   //used to store the command which will show the wallpapers
                snprintf(fehcommand, sizeof(fehcommand), "feh --bg-scale");

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
                                snprintf(path, sizeof(path), "%s%s", configpath, outputInfo->name);

                                FILE *getpicture = fopen(path, "r"); 
                                if(!getpicture){
                                        DIR *direct;
                                        struct dirent *dir;
                                        direct = opendir(wallpaperspath);
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
                                strncat(fehcommand, wallpaperspath, sizeof(fehcommand) - strlen(fehcommand) - 1);
                                strncat(fehcommand, picture, sizeof(fehcommand) - strlen(fehcommand) - 1);
                        }

                        XRRFreeOutputInfo(outputInfo);
                }

                strncat(fehcommand," --no-fehbg" , sizeof(fehcommand) - strlen(fehcommand) - 1);

                system(fehcommand);             // execute the command that shows the wallpapers

                XRRFreeScreenResources(screenResources);
                XCloseDisplay(display);
	}

	return 0;
}
