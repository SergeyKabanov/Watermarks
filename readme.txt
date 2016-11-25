USAGE:

With the command string parameters:

WaterMarks.exe <background image name> <watermark image name>

Example:

WaterMarks.exe flowers.tif watermark.png

If any of these files is missed or has a wrong format, the program starts without loading files.
If the program does not have command string parameters, it starts without loading files as well.
You can load the files manually using the buttons "Load Background File" and "Load Watermark File". 

WaterMarks supports ANY type of image files,
however, the demo version does not allow you to save the result into a file.

WaterMarks creates a semi-transparent watermark by blending the watermark image with the background image.
You can change the transparent color of the watermark image by pressing the button "Choose Transparent Color".
You also can change the transparency level (from 0 (completely invisible) to 255 (completely opaque)).

WaterMarks supports 3 different modes of watermark arrangement:

1). Center - the watermark image is always on the center of the screen and does not change it's size, 
independently of the background file stretch.

2). Fit To Page - the watermark image always stretches to the size of the background image.

3). Scale To Fit - the watermark image changes it's size (but not shape) 
according to the background image size. 
