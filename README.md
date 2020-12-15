# New Blend Modes Testing

The source and project files (Xcode/Mac OSX only, sorry!) for the app that I wrote to test all the blend modes I’m adding to the Cinder fork.

(Mainly because I seem to be having real difficulties with Saturation, Colour, and Luminosity. Something to do with my conversions from HSV to HSL, to RGB, etc, I suspect. But that’s another story.)

Requires the 0.9.3 version for Im::Gui, together with the little patch that fixes the Im::Gui exception when resizing the window.

And of course, also all the extra ip:: blend modes I’m in the process of adding.

The app allows creation of layers which can either be solid colours or images from files on a local disk. Each layer has its own settings for blend mode.

The “document” is saved as a JSON file to my desktop in between runs. At the moment I’ve been lazy and just hardcoded the path to my desktop into the app, so that would also need changing for your system.

It’s all very rough-and-ready ...

