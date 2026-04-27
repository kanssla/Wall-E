# Wall-E

A small Wallpaperprogramm that changes the wallpaper on the screen your mouse is currently on.  

The current setup works as follows:
1. Have a ~Documents/wallpapers directory with some pictures and nothing else than pictures Feh accepts in it (the install script creates one and puts an example image inside).
2. Executing sw in the terminal should set the wallpaper if there exist files for each monitor in ~.config/cw/ which have the name of your monitor and contain the name of the wallpaper thats currently set.
3. These files get created on executing cw or sw.
4. Now cw changes the wallpapers of the monitor your mouse is in and sw sets them to the last change(sw in .xinitrc f.e. reloads your last setup).

tldr:
- cw changes wallpaper in the monitor your mouse is in
- sw sets the wallpaper according to last changes (f.e. run on startup)

I personally have a keyboard shortcut for cw so i can move the cursor to the monitor i want to change the background image of and do that.

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
Now executing
```
    make install-user
```
creates a cw folder in ~.config where the information about which monitor has which wallpaper will be stored as well as a Documents/wallpapers folder where it will store one example Wallpaper.

Now executing
```
    sudo make install-system
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
    sudo make uninstall-system
    make uninstall-user
```
Now everything left to do is to delete the Wall-E folder with its content.

## Sources
The wallpaper that comes with install is from https://wallpapersafari.com/w/0VpyEC/download
