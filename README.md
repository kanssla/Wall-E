# Wall-E

A small Wallpaperprogramm i wrote to have the functionalities i wanted. However i might add some more if i bother to. 

The current setup works as follows:
1. Have a ~Documents/wallpapers directory with some pictures and nothing else than pictures Feh accepts in it.
2. Executing sw in the terminal should set the wallpaper if there exist files for each monitor in ~.config/cw/ which have the name of your monitor and contain a number.
3. On install (see install) Wall-E should create such a file for the monitor you have installed it in (considering you were using X11 at that moment.
4. Executing cw in a Monitor of your choice creates such a file as well. 
5. Now cw changes the wallpapers of the monitor your mouse is in and sw sets them to the last change(sw in .xinitrc f.e. reloads your last setup).

tldr:
- cw changes wallpaper in the monitor your mouse is in
- sw sets the wallpaper according to last changes (f.e. run on startup)

I personally have a keyboard shortcut for cw so i can move the cursor to the monitor i want to change the background image of and do that.

## Requirements
- of course you need Feh (and by this all the usual X11 stuff)
- xrandr
- xdotool

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
creates a cw folder in ~.config where the information about which monitor has which wallpaper will be stored.

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
