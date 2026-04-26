#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include "getmonitor.h"



int inmonitor(int *mouse, int *bl, int *tr){
	if(mouse[0] >= bl[0] && tr[0] >= mouse[0] && mouse[1] >= bl[1] && tr[1] >= mouse[1]){
		return 1;
	}
	return 0;
}

char* getmonitor(){

	char buffer[256];
	char *end;
	int count;
	int mouse[2];
	int bl[2];
	int tr[2];


	Display *display = XOpenDisplay(NULL);
	if(!display){
		printf("Unable to open display\ni");
		return NULL;
	}

	Window root = DefaultRootWindow(display);

	int win_x, win_y;
	unsigned int mask;
	Window root_return, child_return;

	Bool success = XQueryPointer(display, root, &root_return, &child_return, &mouse[0], &mouse[1], &win_x, &win_y, &mask);

	if(!success){
		printf("Failed to query pointer position.\n");
		XCloseDisplay(display);
		return NULL;
	}

	XRRScreenResources *screenResources = XRRGetScreenResources(display, root);

	if(!screenResources){
		printf("Unable to get screen resources\n");
		XCloseDisplay(display);
		return NULL;
	}
	
	for(int i = 0; i < screenResources->noutput; ++i){
		XRROutputInfo *outputInfo = XRRGetOutputInfo(display, screenResources, screenResources->outputs[i]);
		if(outputInfo->connection == RR_Connected){
			XRRCrtcInfo *crtcInfo = XRRGetCrtcInfo(display, screenResources, outputInfo->crtc);
			if(crtcInfo){
				bl[0] = crtcInfo->x; 
				bl[1] = crtcInfo->y; 
				tr[0] = crtcInfo->x + crtcInfo->width; 
				tr[1] = crtcInfo->y + crtcInfo->height; 
				if(inmonitor(mouse, bl, tr)){
					size_t len = snprintf(NULL, 0,"%s", outputInfo->name)+1;
					char* str = malloc(len);
					if(str) snprintf(str, len, "%s", outputInfo->name);
					return str;
				}

				XRRFreeCrtcInfo(crtcInfo);

			}
		}

		XRRFreeOutputInfo(outputInfo);
	}

	XRRFreeScreenResources(screenResources);
	XCloseDisplay(display);

	printf("Mouse not to be found.\n");
	return NULL;
}
