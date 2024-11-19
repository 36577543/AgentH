#include "Tool.h"
#include "Config.h"
#include <windows.h>
#include <wingdi.h>     // BITMAPINFOHEADER
#include <string>
#include <iomanip>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <algorithm> // for std::find_if
#include <codecvt>

// 去掉字符串头部的空格
std::string ltrim(const std::string& str)
{
    auto it = std::find_if(str.begin(), str.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        });
    return std::string(it, str.end());
}

// 去掉字符串尾部的空格
std::string rtrim(const std::string& str)
{
    auto it = std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        });
    return std::string(str.begin(), it.base());
}

// 去掉字符串头尾的空格
std::string trim(const std::string& str)
{
    return rtrim(ltrim(str));
}

std::string WideByte2Acsi(std::wstring& wstrcode)
{
    int asciisize = ::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, NULL, 0, NULL, NULL);
    if (asciisize == ERROR_NO_UNICODE_TRANSLATION)
    {
        throw std::exception("Invalid UTF-8 sequence.");
    }
    if (asciisize == 0)
    {
        throw std::exception("Error in conversion.");
    }
    std::vector<char> resultstring(asciisize);
    int convresult = ::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, &resultstring[0], asciisize, NULL, NULL);
    if (convresult != asciisize)
    {
        throw std::exception("La falla!");
    }
    return std::string(&resultstring[0]);
}

std::wstring Utf8ToUtf16(const std::string& utf8str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(utf8str);
}

std::string Utf8ToAsc(std::string& strUtf8Code)
{
    std::wstring wstr = Utf8ToUtf16(strUtf8Code);
    return WideByte2Acsi(wstr);
}

//cv::Mat CaptureScreen()
//{
//    // Get the desktop window handle
//    HWND hDesktopWnd = GetDesktopWindow();
//
//    // Get the desktop dimensions
//    RECT screenRect;
//    GetClientRect(hDesktopWnd, &screenRect);
//    int width = screenRect.right - screenRect.left;
//    int height = screenRect.bottom - screenRect.top;
//
//    // Create a device context (DC) for the entire screen
//    HDC hScreenDC = GetDC(NULL);
//
//    // Create a compatible DC to hold the bitmap
//    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
//
//    // Create a compatible bitmap to hold the screenshot
//    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
//
//    // Select the compatible bitmap into the compatible DC
//    SelectObject(hMemoryDC, hBitmap);
//
//    // Copy from the desktop DC to the compatible DC using BitBlt
//    BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);
//
//    // 从位图创建OpenCV图像
//    BITMAPINFOHEADER bi;
//    bi.biSize = sizeof(BITMAPINFOHEADER);
//    bi.biWidth = width;
//    bi.biHeight = -height; //负数表示从上到下的图像
//    bi.biPlanes = 1;
//    bi.biBitCount = 32;
//    bi.biCompression = BI_RGB;
//    bi.biSizeImage = 0;
//    bi.biXPelsPerMeter = 0;
//    bi.biYPelsPerMeter = 0;
//    bi.biClrUsed = 0;
//    bi.biClrImportant = 0;
//
//    BITMAPINFO bmi;
//    bmi.bmiHeader = bi;
//    cv::Mat image(height, width, CV_8UC4, cv::Scalar(0, 0, 0, 0));
//    GetDIBits(hMemoryDC, hBitmap, 0, height, image.data, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);
//
//    DeleteObject(hBitmap);
//    DeleteDC(hMemoryDC);
//    ReleaseDC(NULL, hScreenDC);
//    return image;
//}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    char className[256];
    char windowTitle[256];
    GetClassName(hwnd, className, sizeof(className));
    GetWindowText(hwnd, windowTitle, sizeof(windowTitle));
    if (std::string(windowTitle).find("YX游戏") != std::string::npos)
    {
        *(HWND*)lParam = hwnd;
        return FALSE;   // 终止遍历
    }
    return TRUE;        // 继续遍历
}

HWND FindWindows()
{
    HWND hwnd = NULL;
    EnumWindows(EnumWindowsProc, (LPARAM)&hwnd);
    return hwnd;
}

void MouseMove(int x, int y)
{
    POINT point = { x, y };
    ClientToScreen(0, &point);
    SetCursorPos(point.x, point.y);

    GetCursorPos(&point);
    LOG4CPLUS_TRACE(log4cplus::Logger::getInstance("main"), "MouseMove\t" << point.x << ':' << point.y);
    Sleep(2000);
}

void MouseMove(int rect[4])
{
    int x = rect[0] + rand() % rect[2];
    int y = rect[1] + rand() % rect[3];
    MouseMove(x, y);
}

void MouseClick(int sleepms)
{
    mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
    POINT point = { 0 };
    GetCursorPos(&point);
    LOG4CPLUS_TRACE(log4cplus::Logger::getInstance("main"), "MouseClick\t" << point.x << ':' << point.y);
    Sleep(sleepms);
}

void MouseDoubleClick()
{
    mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
    Sleep(100);
    mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
    POINT point = { 0 };
    GetCursorPos(&point);
    LOG4CPLUS_TRACE(log4cplus::Logger::getInstance("main"), "DoubleClick\t" << point.x << ':' << point.y);
}

void MouseScroll(int lines)
{
    int step = 1;
    for (int i = 0; i < lines; i += step)
    {
        mouse_event(MOUSEEVENTF_WHEEL, 0, 0, -step * WHEEL_DELTA, 0);
        Sleep(200);
    }
}

void PressKey(BYTE bVk)
{
    ::keybd_event(bVk, 0, 0, 0);
    ::keybd_event(bVk, 0, KEYEVENTF_KEYUP, 0);
}

void PressStr(const char* str)
{
    std::stringstream ss;
    for (unsigned i = 0; i < strlen(str); i++)
    {
        if (str[i] > 0x60 && str[i] < 0x7B)
            PressKey(str[i] - 0x20);
        else
            PressKey(str[i]);

        int sleep = rand() % 200 + 100;
        ss << sleep << ' ';
        Sleep(sleep);     // anticheat
    }
    Sleep(rand() % 2000);
    LOG4CPLUS_TRACE(log4cplus::Logger::getInstance("main"), "PressStr\t" << str << "\tSleep:" << ss.str());
}

std::vector<BYTE> CaptureWindowToBitmap(HWND hwnd)
{
    HDC hdcScreen = GetDC(NULL);
    HDC hdcWindow = GetWindowDC(hwnd);
    RECT rect;
    GetClientRect(hwnd, &rect);

    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    HDC hdcMemScreen = CreateCompatibleDC(hdcScreen);
    HBITMAP hbmScreen = CreateCompatibleBitmap(hdcScreen, width, height);

    SelectObject(hdcMemScreen, hbmScreen);

    BitBlt(hdcMemScreen, 0, 0, width, height, hdcWindow, 0, 0, SRCCOPY);

    BITMAP bm;
    GetObject(hbmScreen, sizeof(BITMAP), &bm);

    BITMAPINFOHEADER bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bm.bmWidth;
    bi.biHeight = bm.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    DWORD dwBitsSize = ((bm.bmWidth * bi.biBitCount + 31) / 32) * 4 * bm.bmHeight;

    BITMAPFILEHEADER bmfh;
    bmfh.bfType = 0x4D42; // "BM"
    bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwBitsSize;
    bmfh.bfReserved1 = 0;
    bmfh.bfReserved2 = 0;
    bmfh.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

    std::vector<BYTE> buf(bmfh.bfSize, 0);
    BYTE* pBits = buf.data();
    GetDIBits(hdcScreen, hbmScreen, 0, height, pBits, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    DeleteObject(hbmScreen);
    DeleteDC(hdcMemScreen);
    ReleaseDC(NULL, hdcScreen);
    ReleaseDC(hwnd, hdcWindow);
    return buf;
}
