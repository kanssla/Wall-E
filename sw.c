#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


int main(int argc, char *argv[]){
	if(argc == 1){
		
		char *monitor;
		char path[512];
		char wallpaperspath[256];
		snprintf(wallpaperspath, sizeof(wallpaperspath), "ls %s/Documents/wallpapers/", getenv("HOME"));
		char betweenpath[256];
		snprintf(betweenpath, sizeof(betweenpath), "%s/Documents/wallpapers/", getenv("HOME"));
		char fehcommand[512];
		snprintf(fehcommand, sizeof(fehcommand), "feh --bg-scale");
		char buffer[256];
		char moninfo[256];
		char *end;
		int picture = 0;
		int count = 0;
		

		FILE *getmonitors = popen("xrandr --listmonitors", "r");
		if(!getmonitors){
			printf("ERROR: not able to execute xrandr --listmonitors\n");
			return 1;
		}
		while(fgets(moninfo, sizeof(moninfo), getmonitors)){

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

			FILE *getpicture = fopen(path, "r");
			if(!getpicture){
				printf("Not found a Wallpaper for %s.\n", monitor);
				continue;
			}
			while(fgets(buffer, sizeof(buffer), getpicture)){
				picture = strtol(buffer, &end, 10);
			}
			fclose(getpicture);

			FILE *getfilenames = popen(wallpaperspath, "r");
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

		snprintf(path, sizeof(path), "%s --no-fehbg", fehcommand);

		system(path);

	}else{
		printf("No argument expected\n");
		return 1;
	}

	return 0;
}
