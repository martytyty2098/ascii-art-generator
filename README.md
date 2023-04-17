# ascii-art-generator
C++ program that converts a given image to a grayscale ASCII art.\
This program is built on pure C++ with the help of OpenCV library.
# Usage
Download `ascii_generator.zip`\
Put the exe file and opencv_world450.dll in the same folder\
Open an exe file and insert the full path to an image with its name and extension\
Example : 
```
C:\folder_name\some_other_folder\image_name.png (or any other available extension like jpg or bmp)
```
After that, you will see ASCII art that will fill the entire console window.\
You can resize the console window at any time as you wish, the image will also adapt and resize.
# Shorthands
## `-sh`
Type `-sh` after image name to produce an ASCII art with sharp edges\
Example :
```
C:\folder_name\some_other_folder\image_name.png -sh
```
## `-sm`
Type `-sm` after image name to produce an ASCII art with smooth edges\
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
# Examples
**Source image:**

![simpson](https://user-images.githubusercontent.com/108870368/223789561-5baf4af8-5278-48cd-a8c3-3324820dc806.png)

**Command:**
```
C:\photoshop\MyPictures\simpson.png
```
**Program output:**

<img width="979" alt="ascii_simpson" src="https://user-images.githubusercontent.com/108870368/223790077-a1176ada-6c38-428c-9bf1-12c07a86defb.png">

**Source image:**

![adidas](https://user-images.githubusercontent.com/108870368/223790360-d4b9e1c5-c8ae-47bf-8335-5b14919cacc8.jpg)

**Command:**
```
C:\photoshop\MyPictures\adidas.png -sh
```
**Program output:**

<img width="979" alt="Ashampoo_Snap_8 марта 2023 г _21h34m27s_005_" src="https://user-images.githubusercontent.com/108870368/223790468-fc1ab776-a062-4c49-a446-67445149d84d.png">
