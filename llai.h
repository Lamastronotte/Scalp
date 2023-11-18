// it's a small file to do loadlibA injection, i'm working to manual mapping ones but i'm not done yet lol

#pragma once

#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <libloaderapi.h>

using namespace std;

namespace llai
{
    bool fileExist(string file_name);
    void error(const char* error_title, const char* error_message);
    void get_procID(const char* window_title, DWORD& process_id);

    bool loadDll(const char* window_title, char dll_path[MAX_PATH]);
}
