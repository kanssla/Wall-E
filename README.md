# Wall-E

A small Wallpaperprogramm that changes the wallpaper on the screen your mouse is currently on.  

The current setup works as follows:
1. Put wallpapers Feh accepts into ~Documents/wallpapers/ (the install script creates one and puts an example image inside).
2. Executing walle in the terminal should set the wallpaper.
4. Now wlle change changes the wallpapers of the monitor your mouse is in and walle sets them to the last change(walle in .xinitrc f.e. reloads your last setup).

tldr:
- walle change changes wallpaper in the monitor your mouse is in
- walle sets the wallpaper according to last changes (f.e. run on startup)

I personally have a keyboard shortcut for walle change so i can move the cursor to the monitor i want to change the background image of and do that.

## Requirements
- of course you need Feh (and by this all the usual X11 stuff)
- as well as libx11-dev libxrandr-dev

## Install
Clone the repository.
```
    git clone https://github.com/kanssla/Wall-E.git
```
Change directory to the repo.
```
    cd Wall-E
```
Now execute
```
    make 
```
and optionally edit the config.h.

Now executing
```
    sudo make install
```
installs the binaries to /usr/local/bin.

Optionally run
```
    make  clean
```
to remove the binaries from the current folder.

## Uninstall
Go into the Wall-E directory and run
```
    sudo make uninstall
```
Now everything left to do is to delete the Wall-E folder with its content.

## Sources
The wallpaper that comes with install is from https://wallpapersafari.com/w/0VpyEC/download
