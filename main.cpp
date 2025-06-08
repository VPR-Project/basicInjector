#include <windows.h>
#include <iostream>
#include <string>

bool InjectDLL(DWORD pid, const std::string& dllPath) {
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProc) {
        std::cerr << "[-] Can't open process: " << pid << "\n";
        return false;
    }

    void* remoteMem = VirtualAllocEx(hProc, nullptr, dllPath.size() + 1, MEM_COMMIT, PAGE_READWRITE);
    if (!remoteMem) {
        std::cerr << "[-] Failed to allocate memory in target process.\n";
        CloseHandle(hProc);
        return false;
    }

    if (!WriteProcessMemory(hProc, remoteMem, dllPath.c_str(), dllPath.size() + 1, nullptr)) {
        std::cerr << "[-] Failed to write DLL path to target process.\n";
        VirtualFreeEx(hProc, remoteMem, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return false;
    }

    FARPROC loadLibAddr = GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    if (!loadLibAddr) {
        std::cerr << "[-] Failed to get LoadLibraryA address.\n";
        VirtualFreeEx(hProc, remoteMem, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return false;
    }

    HANDLE hThread = CreateRemoteThread(hProc, nullptr, 0, (LPTHREAD_START_ROUTINE)loadLibAddr, remoteMem, 0, nullptr);
    if (!hThread) {
        std::cerr << "[-] Failed to create remote thread.\n";
        VirtualFreeEx(hProc, remoteMem, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return false;
    }

    WaitForSingleObject(hThread, INFINITE);
    VirtualFreeEx(hProc, remoteMem, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProc);

    return true;
}

int main(int argc, char* argv[]) {
    DWORD pid = 0;
    std::string dllPath;

    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-p") && i + 1 < argc)
            pid = std::stoi(argv[++i]);
        else if (!strcmp(argv[i], "-f") && i + 1 < argc)
            dllPath = argv[++i];
    }

    if (pid == 0 || dllPath.empty()) {
        std::cout << "Usage: injector.exe -p <PID> -f <DLL path>\n";
        return 1;
    }

    std::cout << "[*] Injecting " << dllPath << " into PID " << pid << "...\n";
    if (InjectDLL(pid, dllPath)) {
        std::cout << "[+] Injection successful.\n";
    }
    else {
        std::cerr << "[-] Injection failed.\n";
    }

    return 0;
}
