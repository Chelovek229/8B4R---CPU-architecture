// boot_logo.cpp
// ���������� ASCII-������� 8B4R � ��� Ctrl+C+F12 ��������� Cynita Loader (cynita_loader.exe).
// Windows-only.

#include <iostream>
#include <string>
#include <windows.h>
#include <thread>
#include <atomic>
#include <shlobj.h>

static const char* ascii_logo = R"(
  ####    #####     ##     #####  
 #    #   #    #   # #    #    # 
 #    #   #    #  #   #   #    # 
  ####    #####   ## ##    #####  
 #    #   #    #  ######  #  #    
 #    #   #    #      #   #   #   
  ####    #####       #   #    #  
)";

static std::atomic<bool> cynita_mode(false);

BOOL WINAPI ConsoleHandler(DWORD ctrlType) {
    if (ctrlType == CTRL_C_EVENT) {
        // ����������, ����� ���������� ��������� �������
        return TRUE;
    }
    return FALSE;
}

void clear_console() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h == INVALID_HANDLE_VALUE) return;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD written;
    if (!GetConsoleScreenBufferInfo(h, &csbi)) return;
    DWORD cells = csbi.dwSize.X * csbi.dwSize.Y;
    COORD home = { 0, 0 };
    FillConsoleOutputCharacterA(h, ' ', cells, home, &written);
    SetConsoleCursorPosition(h, home);
}

void draw_screen() {
    clear_console();
    std::cout << ascii_logo << "\n";
    std::cout << "CPU architecture by KiG Orgsnization\n\n";
    std::cout << "Cynita mode: " << (cynita_mode ? "On" : "Off") << "\n\n";
    std::cout << "Press Ctrl + C + F12 to open Cynita Loader.\n";
    std::cout << "Press ESC to exit this screen.\n";
}

static std::string get_exe_folder() {
    char path[MAX_PATH];
    if (GetModuleFileNameA(NULL, path, MAX_PATH) == 0) return std::string(".");
    std::string p(path);
    size_t pos = p.find_last_of("\\/");
    if (pos == std::string::npos) return std::string(".");
    return p.substr(0, pos);
}

void launch_cynita_loader() {
    std::string exeFolder = get_exe_folder();
    std::string loaderPath = exeFolder + "\\cynita_loader.exe";

    // ���� ����������� ���� � ��� �� ����� �� ������ - ��������� �� ������� ����������
    if (GetFileAttributesA(loaderPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
        loaderPath = "cynita_loader.exe";
        if (GetFileAttributesA(loaderPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
            std::cerr << "Cynita Loader not found: expected " << exeFolder << "\\cynita_loader.exe\n";
            return;
        }
    }

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    BOOL ok = CreateProcessA(loaderPath.c_str(), NULL, NULL, NULL, FALSE, 0, NULL, exeFolder.c_str(), &si, &pi);
    if (!ok) {
        std::cerr << "Failed to launch Cynita Loader (" << GetLastError() << ")\n";
        return;
    }
    // �������, ���� GUI ��������� (BIOS-�������� ���������)
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main() {
    SetConsoleCtrlHandler(ConsoleHandler, TRUE);
    draw_screen();

    bool prev_combo = false;

    while (true) {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            break;
        }

        bool ctrl = (GetAsyncKeyState(VK_LCONTROL) & 0x8000) || (GetAsyncKeyState(VK_RCONTROL) & 0x8000);
        bool keyC = (GetAsyncKeyState('C') & 0x8000) || (GetAsyncKeyState('c') & 0x8000);
        bool f12 = (GetAsyncKeyState(VK_F12) & 0x8000);

        bool combo = ctrl && keyC && f12;

        if (combo && !prev_combo) {
            cynita_mode = true;
            draw_screen();
            // ��������� GUI
            launch_cynita_loader();
            // ����� �������� GUI � ������� ���� � ���������� �������
            cynita_mode = false;
            draw_screen();
            // ���� ���������� ������
            while ((GetAsyncKeyState(VK_F12) & 0x8000) || (GetAsyncKeyState('C') & 0x8000)
                   || (GetAsyncKeyState(VK_LCONTROL) & 0x8000) || (GetAsyncKeyState(VK_RCONTROL) & 0x8000)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            prev_combo = false;
        } else {
            prev_combo = combo;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    SetConsoleCtrlHandler(ConsoleHandler, FALSE);
    return 0;
}