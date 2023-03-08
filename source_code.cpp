#include <opencv2/opencv.hpp>
#include <iostream>
#include <windows.h>
#include <cwchar>
#include <math.h>
#include <unordered_map>
#include <string>
#include <lmcons.h>

// ASCII characters from darkest to brightest
unsigned char colors[] = " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";
constexpr int ColorsAmount = 91;

// an array that stores the precomputed brightness value for each possible RGB value
// i know its not memory efficient because its about 67 megabytes, but at least 
// the program doesn't have to do expensive floating point calculations during image generation
float brightnessLookup[256 * 256 * 256];

// Rec. 601 standard
constexpr float WeightR = 0.299f;
constexpr float WeightG = 0.587f;
constexpr float WeightB = 0.114f;

#define R 2
#define G 1
#define B 0

// The darkest is 0 and the brightest is 1
#define FindBrightness(r, g, b) ((r / 255.0f * WeightR) + (g / 255.0f * WeightG) + (b / 255.0f * WeightB))

// Console window size in characters
// Each character has 16 pixels in height and 8 pixels in width by default (in "Consolas" font)
void GetWindowSize(int& width_out, int& height_out)
{
    HANDLE OutputBufferHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO OutputBufferInfo = {};
    GetConsoleScreenBufferInfo(OutputBufferHandle, &OutputBufferInfo);
    width_out = OutputBufferInfo.srWindow.Right - OutputBufferInfo.srWindow.Left + 1;
    height_out = OutputBufferInfo.srWindow.Bottom - OutputBufferInfo.srWindow.Top + 1;
}

unsigned char FindAverageColor(const cv::Mat* const image, int x_originPix, int y_originPix, int ChunkWidth, int ChunkHeight)
{
    float AvgBrightness = 0.0f;
    for (int yPix = y_originPix; yPix < y_originPix + ChunkHeight; ++yPix)
    {
        for (int xPix = x_originPix; xPix < x_originPix + ChunkWidth; ++xPix)
        {
            const unsigned char* CurrentPix = image->ptr<unsigned char>(yPix) + 3 * xPix;
            int index = 
                (int(CurrentPix[R]) << 16) |
                (int(CurrentPix[G]) << 8) |
                int(CurrentPix[B]); // combine RGB value into a single integer
            AvgBrightness += brightnessLookup[index];
        }
    }

    AvgBrightness /= ChunkWidth * ChunkHeight;
    return colors[int(ColorsAmount * AvgBrightness)];
}

unsigned char FindDominantColor(const cv::Mat* const image, int x_originPix, int y_originPix, int ChunkWidth, int ChunkHeight)
{
    // a map that stores the brightness value(float) and the number of times it has occurred in the current chunk(int)
    std::unordered_map<float, int> ColorsTable;

    for (int yPix = y_originPix; yPix < y_originPix + ChunkHeight; ++yPix)
    {
        for (int xPix = x_originPix; xPix < x_originPix + ChunkWidth; ++xPix)
        {
            const unsigned char* CurrentPix = image->ptr<unsigned char>(yPix) + 3 * xPix;
            int index = 
                (int(CurrentPix[R]) << 16) | 
                (int(CurrentPix[G]) << 8) | 
                int(CurrentPix[B]); // combine RGB value into a single integer

            float CurrentPixBrightness = brightnessLookup[index];
            ++ColorsTable[CurrentPixBrightness];
        }
    }

    float DominantColor = 0.0f;
    int MaxEncounterAmount = 0;

    // finds the brightness value that most often occurred in the current chunk
    for (auto i = ColorsTable.begin(), end = ColorsTable.end(); i != end; ++i) 
    {
        if (i->second > MaxEncounterAmount)
        {
            MaxEncounterAmount = i->second;
            DominantColor = i->first;
        }
    }

    return colors[int(ColorsAmount * DominantColor)];
}

// this function breaks down the source image into chunks and processes each chunk
// then it fills a given array of characters with the results of processing each chunk
// the the total amount of chunks is always WinWidth * WinHeight
void LoadImageToBuffer(const cv::Mat* const SourceImage, unsigned char* const DestBuffer, int WinWidth, int WinHeight, bool SharpEdges)
{
    const float ChunkWidth = SourceImage->cols / float(WinWidth);
    const float ChunkHeight = SourceImage->rows / float(WinHeight);

    // searching for dominant color in the chunk makes an image with sharp edges and fewer color amount
    // searching for average color in the chunk makes an image with smooth edges and more color amount
    unsigned char (*FindColor)(const cv::Mat*, int, int, int, int) = SharpEdges ? &FindDominantColor : &FindAverageColor;

    // fills an array of characters with the results of processing each chunk
    for (int y = 0; y < WinHeight; ++y)
    {
        for (int x = 0; x < WinWidth; ++x)
        {
            DestBuffer[WinWidth * y + x] = (*FindColor)
                (
                SourceImage,
                int(ChunkWidth * x),
                int(ChunkHeight * y),
                int(ceilf(ChunkWidth)),
                int(ceilf(ChunkHeight))
                );
        }
    }

    DestBuffer[WinWidth * WinHeight] = '\0';
}

// get username in order to access their desktop
std::string GetUsername()
{
    TCHAR username[UNLEN + 1] = {};
	DWORD size = UNLEN + 1;
	std::string current_username = "";
	GetUserName((TCHAR*)username, &size);
	for (unsigned long i = 0; i < size; ++i) {
		current_username += (char)username[i];
	}
	return current_username;
}

// precomputes the brightness value for each possible RGB value
constexpr void ComputeBrightnessLookupTable()
{
    for (int r = 0; r < 256; ++r) {
        for (int g = 0; g < 256; ++g) {
            for (int b = 0; b < 256; ++b)
            {
                int index = (r << 16) | (g << 8) | b;
                brightnessLookup[index] = FindBrightness(r, g, b);
            }
        }
    }
}

// returns false if str doesnt contain substr
bool RemoveFromString(std::string& str, std::string substr)
{
    bool FoundSomething = false;
    size_t index = str.find(substr, 0);
    while (index >= 0 && index < str.length())
    {
        str.erase(index, substr.length());
        index = str.find(substr, 0);
        FoundSomething = true;
    }
    return FoundSomething;
}

// manage shorthands and spaces
void ProcessUserInput(std::string& Input, bool& SharpEdges)
{
    // remove all spaces at the beginning of a string
    for (int i = 0; Input[i] != '\\' && i < Input.length(); ++i)
    {
        if(Input[i] == ' ') Input.erase(0, 1);
    }

    SharpEdges = RemoveFromString(Input, "-sh");
    RemoveFromString(Input, "-sm");

    // erase all zeros that somehow appear in the string but i don't know why
    for (int i = 0; i < Input.length(); ++i) {
        if (int(Input[i]) == 0) {
            Input.erase(i, 1);
            --i;
        }
    }
}

int main()
{
    ComputeBrightnessLookupTable();

    std::string image_path = "";
    std::string username = GetUsername();
    std::string DesktopPath = "C:\\Users\\" + username + "\\Desktop\\";

    // Console window size in characters (120 width and 30 height by default)
    int win_width = 120;
    int win_height = 30;

    bool SharpEdges = false;

    std::cout << "Insert the path of your image\n\n";
    std::cout << "Shorthands :\n-sh : Keep sharp edges\n-sm : Smooth edges (by default)\n-d : Shorthand for your desktop\n\n";
    std::cout << "Example :\nC:\\folder_name\\some_other_folder\\image_name.png -sm\n\n";
    std::cout << "If the image is on your desktop, just type \"-d\"\n";
    std::cout << "(If you want to resize your console window, do it now)\n\n";
    do {
        std::getline(std::cin, image_path, '\n');
    } while (image_path.empty());

    // read from desktop
    if (RemoveFromString(image_path, "-d")) {
        std::cout << "\nNow insert the name of your image with its extension\n";
        std::cout << "Example : image_name.jpg -sm (any other extension is also valid)\n";
        do {
            std::getline(std::cin, image_path, '\n');
        } while (image_path.empty());
        image_path = DesktopPath + image_path;
    }

    ProcessUserInput(image_path, SharpEdges);
    std::cout << "Opening image at " << image_path << '\n';

    cv::Mat SourceImage = cv::imread(image_path);

    if (SourceImage.empty())
    {
        std::cout << "\nThe image at " << image_path << " was not found.\n";
        std::cout << "This address is probably invalid or the file extension is not supported\n";
        std::cout << "\nPress Enter to exit . . . ";
        std::cin.get();
        return -1;
    }

    GetWindowSize(win_width, win_height);
    int last_width = win_width;
    int last_height = win_height;

    unsigned char* ImageBuffer = new unsigned char[win_width * win_height + 1];
    LoadImageToBuffer(&SourceImage, ImageBuffer, win_width, win_height, SharpEdges);

    std::cout << ImageBuffer;

    // if the width or height of the last frame does not match the current frame
    // it means the window has been resized
    while (true)
    {
        GetWindowSize(win_width, win_height);
        if (last_width != win_width || last_height != win_height)
        {
            for (int i = 0; i < win_height; ++i) {
                std::cout << '\n';
            }
            std::cout << "Resizing...\nMay take some time if the image is large\n";
            delete[] ImageBuffer;
            ImageBuffer = new unsigned char[win_width * win_height + 1];
            LoadImageToBuffer(&SourceImage, ImageBuffer, win_width, win_height, SharpEdges);
            std::cout << ImageBuffer;
        }
        last_width = win_width;
        last_height = win_height;
    }

    return 0;
}