#include "llai.h"

namespace llai
{
    void get_procID(const char* window_title, DWORD& process_id)
    {
        GetWindowThreadProcessId(FindWindow(NULL, window_title), &process_id);
    }

    void error(const char* error_title, const char* error_message) // displays an error
    {
        MessageBox(0, error_message, error_title, 0);
    }

    bool fileExist(string file_name)
    {
        struct stat buffer;
        return (stat(file_name.c_str(), &buffer) == 0);
    }

    bool loadDll(const char* window_title, char dll_path[MAX_PATH])
    {
        DWORD proc_id = NULL;

        get_procID(window_title, proc_id);
        if (proc_id == NULL) { error("GetProcID", "Failed to get process id"); return false; }

        HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, NULL, proc_id);
        if (!h_process) { error("OpenProcess", "Failed to open a handle to process"); return false; }

        void* allocated_memory = VirtualAllocEx(h_process, nullptr, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        if (!allocated_memory) { error("VirtualAllocEx", "Failed to allocate memory"); return false; }

        if (!WriteProcessMemory(h_process, allocated_memory, dll_path, MAX_PATH, nullptr)) { error("WriteProcessMemory", "Failed to write process memory"); return false; }

        HANDLE h_thread = CreateRemoteThread(h_process, nullptr, NULL, LPTHREAD_START_ROUTINE(LoadLibraryA), allocated_memory, NULL, nullptr);
        if (!h_thread) { error("CreateRemoteThread", "Faile to create remote thread"); return false; }

        CloseHandle(h_process);
        VirtualFreeEx(h_process, allocated_memory, NULL, MEM_RELEASE);

        return true;
    }

}
