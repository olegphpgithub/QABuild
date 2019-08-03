#pragma once

#include <windows.h>

class ResourceContainer
{
public:
    ResourceContainer(void);
    ~ResourceContainer(void);
    static void extractFileFromResource(DWORD resource_id, LPCTSTR outputFileName);
    static void LoadRTFFromResource(DWORD resource_id, HWND hwnd);
};
