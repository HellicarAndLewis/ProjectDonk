Setup
=====

data/settings/general.xml
-------------------------
There's a file that gets generated called data/settings/general.xml. In here you can decide how many projectors to use, where the OBJ model is, what res to output at, how many kinects there are etc. This info is used to determine the app's final width and height.

Folder Structure
================
apps/3d/lib/ - the general purpose projection mapping/kinect code.
everything else can go in each project folder.

Keys
====
' ' (space) toggles the gui overall
'1' toggles the scene settings controls (this is automatically saved 
	every time you change it
'2' toggles the viewports
'f' toggles fullscreen


Auto-Documentation
==================
If you have Doxygen installed (in /Applications) the xcode project will automatically generate documentation in the docs/ folder.