<!-- badges: start -->
![License](https://img.shields.io/badge/license-MIT-brightgreen)
<!-- badges: end -->

# My build of dwm

> dwm is an extremely fast, small, and dynamic window manager for X window system.

### Requirements

In order to build dwm, you need the Xlib header files and for the statusbar you'll need the `xsetroot` package from Xorg. If you're on an Arch-based distro install `xorg-xsetroot`.

### Patches

I have the following patches installed:

* Systray - for a systray on the statusbar for applets.
* Scratchpads - multiple scratchpads bound to various keys (just like i3wm!).
* Pertag - maintain a layout per tag instead of per monitor (which is the default).
* Noborder - removes borders when a window is present in monocle mode or only one window present in view.
* Vanity gaps - adds inner, outer gaps that can be changed on the fly ~~(but works only in tiled layout, better than tilegap layout)~~ (new patch -> allows gaps in all layouts).
* Push_no_master - moves windows up/down the stack but doesn't push to master, use zoom() instead.
* Notitle - Removes the title section of windows from the statusbar (allows me to add more modules to dwmblocks).
* Focusonactive - patch that for example when opening a url in one tag shifts focus to the tag where the browser is present.
* Reset_mfact (modified resetlayout) - resets mfact on tag when only 1 client is present.
* Moveresize - grants user to move and change the dimensions of floating windows without touching the mouse!
- Alwayscenter - All floating windows are centered without window rules.
- Savefloats - saves floating windows in position and size before forcing it into tiling mode.
- Focusmaster - shift focus to master from any window in the stack using a key-bind.
- Swallow - swallows windows and essentially gives my terminal, the ability to watch videos and view images (in a way).
- Fullscreen compilation - Allows fakefullscreen and fullscreen with much more practicality into it.
- Shiftviewclients - Cycle through non-empty tags.

## Install `libxft-bgra`

This build of dwm does not implement color emoji in the statusbar, so you must install [libxft-bgra](https://aur.archlinux.org/packages/libxft-bgra/). 
