#pragma once
#include <windows.h>
#include "json11.hpp"

std::string trim(const std::string& str);

std::string Utf8ToAsc(std::string& strUtf8Code);

HWND FindWindows();

void MouseMove(int x, int y);

void MouseMove(int rect[4]);

void MouseClick(int sleepms = 0);

void MouseDoubleClick();

void MouseScroll(int lines);

void PressKey(BYTE bVk);

void PressStr(const char* str);

std::vector<BYTE> CaptureWindowToBitmap(HWND hwnd);
