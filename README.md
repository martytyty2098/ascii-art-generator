# ascii-art-generator
C++ program that converts a given image to a grayscale ASCII art
# Usage
Open an exe file and insert the full path to an image with its name and extension.
Example : 
```
C:\folder_name\some_other_folder\image_name.png (or any other available extension like jpg or bmp)
```
After that, you will see ASCII art that will fill the entire console window.

You can resize the console window at any time as you wish, the image will also adapt and resize.
# Shorthands
## `-sh`
Type `-sh` after image name to produce an ASCII art with sharp edges

Example :
```
C:\folder_name\some_other_folder\image_name.png -sh
```
## `-sm`
Type `-sm` after image name to produce an ASCII art with smooth edges

Example:
```
C:\folder_name\some_other_folder\image_name.png -sm
```
This shorthand is used by default so you don't have to type it every time.
## `-d`
If the image is on your desktop, instead of typing 
```
C:\Users\username\Desktop\
```
you can just type 
```
-d (then press enter)
```
And then you will only need to enter the name of the image with its extension and possible shorthand, like -sh or -sm

Like this : 
```
image_name.png -sh
```
