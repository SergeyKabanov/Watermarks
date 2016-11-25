The WaterMarks application is made as Visual Studio 2015 C++ solution for Windows platform (Windows 7,8.1, 10).
It uses statically linked MFC libraries and does need to have MFC-related dlls to run.
However, it needs to have WIC component (WindowsCodecs.dll) and GDI+ component (gdiplus.dll)
to be registered in the system before running, as well as the library "riched32.dll" to support CRichEditCtrl.

WaterMarks supports any type of images.
The special class "CWicImage" takes care of loading and rendering images into device context using WIC interfaces.
Alpha blending with color key (for transparency), however, is done with GDI+ (version 7.1A).

There are 2 archives attached:

1). The source code (VC++ 2015 solution). It requires VC 2015 to be installed on your machine.
Visual Studio 2015 (Enterprise edition) contains WIC, GDI+ and other necessary components to run the application.
The default configuration (Debug, x86) has the command string to start the program as:

WaterMarks.exe flowers.tif watermark.png

Both image files are in the source code archive.

2). The executable file (release, x86), necessary components and the batch file.
To start it properly, you need to use the batch file:

watermarks.bat


Program usage:

With the command string parameters:

WaterMarks.exe <background image name> <watermark image name>

Example:

WaterMarks.exe flowers.tif watermark.png

If any of these files is missed or has a wrong format, the program starts without loading files.
If the program does not have command string parameters, it starts without loading files as well.
You can load the files manually using the buttons "Load Background File" and "Load Watermark File". 

WaterMarks creates a semi-transparent watermark by blending the watermark image with the background image.
You can change the transparent color of the watermark image by pressing the button "Choose Transparent Color".
You also can change the transparency level (from 0 (completely invisible) to 255 (completely opaque)).

WaterMarks supports 3 different modes of watermark arrangement:

1). Center - the watermark image is always on the center of the screen and does not change it's size, 
independently of the background file stretch.

2). Fit To Page - the watermark image always stretches to the size of the background image.

3). Scale To Fit - the watermark image changes it's size (but not shape) 
according to the background image size. 
