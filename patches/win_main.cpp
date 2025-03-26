#include <Windows.h>
#include <d3d9.h>
#include "win_main.hpp"
#include "core/game/cars_game.hpp"
#include "core/globals.hpp"
#include "core/config.hpp"

HINSTANCE* g_hInstance = reinterpret_cast<HINSTANCE*>(0x0071b220);
HWND* g_HWND = reinterpret_cast<HWND*>(0x0071b22c);
HINSTANCE* g_hInstanceReal = reinterpret_cast<HINSTANCE*>(0x0071afdc);
HWND* g_HWNDReal = reinterpret_cast<HWND*>(0x0071afd8);
HANDLE* g_GameMutex = reinterpret_cast<HANDLE*>(0x0071b238);
bool* g_IsWindowed = reinterpret_cast<bool*>(0x0071b224);
int* g_WindowWidth = reinterpret_cast<int*>(0x006faa54);
int* g_WindowHeight = reinterpret_cast<int*>(0x006faa58);
char* g_ClassName = reinterpret_cast<char*>(0x0071b0e0);
char* g_Caption = reinterpret_cast<char*>(0x0071afe0);
char* g_DataPCDirectory = reinterpret_cast<char*>(0x006faa60);
char* g_StreamingDataPCDirectory = reinterpret_cast<char*>(0x00715200);
int* g_GameInitialized = reinterpret_cast<int*>(0x0071b230);
int* g_TickGame = reinterpret_cast<int*>(0x006faa5c);

DeclareFunction(void, __stdcall, ReadConfigIni, 0x00619190);
DeclareFunction(void, __thiscall, CarsGame_PresentFrame, 0x004237a0, CarsGame*, int);
DeclareFunction(void, __cdecl, HandleClick, 0x00618ce0, DWORD, DWORD, DWORD, DWORD);

void get_platform_id_and_d3d_version(unsigned int* direct3dVersion, DWORD* outPlatformId, unsigned int* canUseSoftwareRenderer) {
    HMODULE blade = LoadLibraryA("Blade.dll");
    if (blade == nullptr) {
        *canUseSoftwareRenderer = 0;
    }
    else {
        *canUseSoftwareRenderer = 1;
        FreeLibrary(blade);
    }
    OSVERSIONINFOA version_info{};
    version_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
    if (!GetVersionExA(&version_info)) {
        *direct3dVersion = 0;
        *outPlatformId = 0;
    }
    else if (version_info.dwPlatformId == 0) {
        *direct3dVersion = 0;
        *outPlatformId = 0;
    }
    else {
        *outPlatformId = version_info.dwPlatformId;
        if ((version_info.dwPlatformId == 2) && (version_info.dwMajorVersion < 4)) {
            *outPlatformId = 0;
        }
        HMODULE d3d9_dll = LoadLibraryA("d3d9.dll");
        if (d3d9_dll != nullptr) {
            FreeLibrary(d3d9_dll);
            IDirect3D9* d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
            if (d3d9 != nullptr) {
                d3d9->Release();
                *direct3dVersion = DIRECT3D_VERSION;
                return;
            }
        }
        *direct3dVersion = 0;
    }
    return;
}

LRESULT CALLBACK wnd_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg < 0x11) {
        if (uMsg == 0x10) {
            (*lpGame)->Terminate();
            DestroyWindow(hwnd);
            PostQuitMessage(0);
            return 0;
        }
        switch (uMsg) {
        case 3:
        case 5:
            if ((*g_GameInitialized != 0) && (IsIconic(hwnd) != 0)) {
                (*lpGame)->Deactivate();
                return DefWindowProcA(hwnd, uMsg, wParam, lParam);
            }

        case 6:
            if (((short)wParam != 0) && ((wParam >> 0x10) == 0)) {
                *g_TickGame = 1;
                (*lpGame)->Activate();
                return DefWindowProcA(hwnd, uMsg, wParam, lParam);
            }
            *g_TickGame = 0;
            (*lpGame)->Deactivate();
            return DefWindowProcA(hwnd, uMsg, wParam, lParam);
        }
    }
    else if (uMsg < 0x114) {
        if (uMsg == 0x113) {
            if (*g_TickGame != 0) {
                if ((*lpGame)->Tick() == 0) {
                    (*lpGame)->Terminate();
                    DestroyWindow(hwnd);
                    PostQuitMessage(0);
                    return 1;
                }
                CarsGame_PresentFrame(reinterpret_cast<CarsGame*>(*lpGame), 1);
                return DefWindowProcA(hwnd, 0x113, wParam, lParam);
            }
        }
        else {
            if (uMsg == 0x20) {
                if ((short)lParam == 1) {
                    SetCursor((HCURSOR)0x0);
                    return 1;
                }
                SetCursor(LoadCursorA((HINSTANCE)0x0, (LPCSTR)0x7f00));
                return 1;
            }
            if (uMsg == 0x112) {
                switch (wParam & 0xfffffff0) {
                case 0xf100:
                case 0xf140:
                case 0xf170:
                case 0xf190:
                    if (*g_TickGame != 0) {
                        return 0;
                    }
                }
            }
        }
    }
    else if ((0x1ff < uMsg) && ((uMsg < 0x203 || (uMsg == 0x204)))) {
        HandleClick(uMsg, wParam, (int)(short)lParam, (int)(short)((unsigned int)lParam >> 0x10));
    }
    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

int initialize_window(HINSTANCE hInstance) {

    if (!(*g_IsWindowed) && config::g_WindowedFullscreenEnabled) {
        *g_WindowWidth = GetSystemMetrics(SM_CXSCREEN);
        *g_WindowHeight = GetSystemMetrics(SM_CYSCREEN);
    }

    WNDCLASSA wnd_class;
    wnd_class.style = 8;
    wnd_class.hIcon = nullptr;
    wnd_class.hCursor = nullptr;
    wnd_class.lpfnWndProc = wnd_proc;
    wnd_class.cbClsExtra = 0;
    wnd_class.cbWndExtra = 0;
    wnd_class.hInstance = hInstance;
    wnd_class.hbrBackground = (HBRUSH)GetStockObject(4);
    wnd_class.lpszMenuName = nullptr;
    wnd_class.lpszClassName = g_ClassName;
    RegisterClassA(&wnd_class);

    RECT wnd_rect{};
    wnd_rect.left = 0;
    wnd_rect.top = 0;
    wnd_rect.right = *g_WindowWidth;
    wnd_rect.bottom = *g_WindowHeight;

    DWORD dwStyle = 0;
    if (!(*g_IsWindowed)) {
        dwStyle = WS_SYSMENU | WS_POPUP;
    }
    else {
        dwStyle = WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_GROUP | WS_TABSTOP;
    }

    *g_HWND = CreateWindowExA(0, g_ClassName, g_Caption, dwStyle, 0, 0, *g_WindowWidth, *g_WindowHeight, nullptr, nullptr, hInstance, nullptr);
    if (*g_HWND == nullptr) {
        return 0;
    }

    SetWindowPos(*g_HWND, nullptr, 0, 0, 0, 0, 0x43);
    UpdateWindow(*g_HWND);
    if (*g_IsWindowed) {
        AdjustWindowRect(&wnd_rect, 0xcf0000, 0);
        MoveWindow(*g_HWND, 0, 30, wnd_rect.right - wnd_rect.left, wnd_rect.bottom - wnd_rect.top, 1);
    }

    return 1;
}

int win_main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    unsigned int d3d_version = 0;
    DWORD platform_id = 0;
    unsigned int can_use_sw_renderer = 0;

    *g_hInstance = hInstance;
    if (strstr(lpCmdLine, "-windowed") != nullptr) {
        *g_IsWindowed = true;
    }

    // get_platform_id_and_d3d_version(&d3d_version, &platform_id, &can_use_sw_renderer);
    /*
    MEMORYSTATUSEX memory_status_ex;
    memset(&memory_status_ex, 0, 0x40);
    memory_status_ex.dwLength = 0x40;
    GlobalMemoryStatusEx(&memory_status_ex);

    if ((platform_id != 1) && (platform_id != 2)) {
        MessageBoxA(nullptr, nullptr, "This program cannot be used by this operating system", MB_ICONERROR);
        _exit(0);
    }
    if (d3d_version < 0x900) {
        MessageBoxA(nullptr, nullptr, "You must use DirectX version 9.0 or higher", MB_ICONERROR);
        _exit(0);
    }
    */
    /*
    uVar6 = memory_status_ex.ullTotalPhys._4_4_ + (uint)(-0x2000000 < memory_status_ex.ullTotalPhys._0_4_);
    if ((memory_status_ex.ullTotalPageFile._4_4_ <= uVar6) &&
        ((memory_status_ex.ullTotalPageFile._4_4_ < uVar6 ||
            (memory_status_ex.ullTotalPageFile._0_4_ < memory_status_ex.ullTotalPhys._0_4_ + 0x2000000)))) {
        MessageBoxA(nullptr, nullptr, "This program cannot be used without enabling Virtual Memory", 0x10);
        _exit(0);
    }*/

    strcpy(g_ClassName, "Rainbow Studios");
    strcpy(g_Caption, "Cars");
    setlocale(0, "English");

    std::string data_directory = g_DataDir.string() + "\\";
    std::memset(g_DataPCDirectory, 0, 256);
    std::memset(g_StreamingDataPCDirectory, 0, 260);
    strncpy(g_DataPCDirectory, data_directory.data(), 256);
    strncpy(g_StreamingDataPCDirectory, g_DataPCDirectory, 260);

    ReadConfigIni();

    HANDLE mutex = CreateMutexA(nullptr, 1, "Cars Mater-National Mutex");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        mutex = nullptr;
    }
    *g_GameMutex = mutex;
    if (mutex == nullptr) {
        HWND existing_window = FindWindowA(g_ClassName, g_Caption);
        if (existing_window != nullptr) {
            ShowWindow(existing_window, 9);
        }
        _exit(0);
    }

    if ((*lpGame)->PreWindowInitialize() == 0) {
        _exit(0);
    }
    int return_value = 0;
    if (initialize_window(hInstance) == 0) {
        return_value = 1;
    }
    else {
        *g_HWNDReal = *g_HWND;
        *g_hInstanceReal = hInstance;
        if ((*lpGame)->PreGameInitialize(nullptr) == 0) {
            _exit(0);
        }

        char quit_message[260]{};
        if ((*lpGame)->InitializeRenderer(quit_message) == 0) {

            DestroyWindow(*g_HWND);
            if (((*lpGame)->flags & 2) != 0) {
                ShowCursor(1);
            }
            MessageBoxA(nullptr, quit_message + 4, g_Caption, 0x10);
            PostQuitMessage(0);
        }
        if ((*lpGame)->Initialize() == 0) {
            DestroyWindow(*g_HWND);
            if (((*lpGame)->flags & 2) != 0) {
                ShowCursor(1);
            }
            PostQuitMessage(0);
        }

        *g_GameInitialized = 1;
        bool should_exit = false;
        MSG message{};
        do {
            BOOL message_available = PeekMessageA(&message, nullptr, 0, 0, 1);
            while (message_available != 0) {
                if (message.message == 0x12) {
                    should_exit = true;
                }
                TranslateMessage(&message);
                DispatchMessageA(&message);
                message_available = PeekMessageA(&message, nullptr, 0, 0, 1);
            }
            if ((!should_exit) && (g_TickGame != 0)) {
                if ((*lpGame)->Tick() == 0) break;
                CarsGame_PresentFrame(reinterpret_cast<CarsGame*>(*lpGame), 1);
            }
            Sleep(1);
        } while (!should_exit);
        if (*g_GameMutex != nullptr) {
            ReleaseMutex(*g_GameMutex);
        }
        *g_GameMutex = nullptr;
        return_value = message.wParam;
    }
    return return_value;
}