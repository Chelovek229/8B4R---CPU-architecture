// cynita_loader.cpp
// Cynita Loader — BIOS-like GUI using Dear ImGui (Win32 + DirectX11).
// Controls: Right Ctrl + F11 = Save & Close; Left Ctrl + F11 = Close without save.
//
// Требования:
// - Dear ImGui + backends: imgui_impl_win32.cpp/.h, imgui_impl_dx11.cpp/.h
// - Линковка: d3d11.lib
//
// Сохраняет настройки в "cynita_settings.ini" и дамп памяти в "cynita_memory.bin" в той же папке, где exe.

#include <windows.h>
#include <d3d11.h>
#include <tchar.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#pragma comment(lib, "d3d11.lib")

// -----------------------------------------------------------------------
// D3D11 global
static ID3D11Device*            g_pd3dDevice = NULL;
static ID3D11DeviceContext*     g_pd3dDeviceContext = NULL;
static IDXGISwapChain*          g_pSwapChain = NULL;
static ID3D11RenderTargetView*  g_mainRenderTargetView = NULL;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// -----------------------------------------------------------------------
// Cynita settings
struct CynitaSettings {
    uint32_t cpu_freq_mhz = 1000;
    float temperature = 36.0f;
    bool fan_enabled = true;
    bool auto_cooling = true;

    int ram_kb = 256;
    int rom_kb = 1024;
    int ddr_kb = 0;
    int sram_kb = 0;
    int dram_kb = 0;
    int cache_kb = 8;

    bool uart_enabled = true;
    bool spi_enabled = true;
    bool i2c_enabled = true;

    std::vector<uint8_t> memory; // byte array, editable
};

static std::string get_exe_folder() {
    char path[MAX_PATH];
    if (GetModuleFileNameA(NULL, path, MAX_PATH) == 0) return ".";
    std::string p(path);
    size_t pos = p.find_last_of("\\/");
    if (pos == std::string::npos) return ".";
    return p.substr(0, pos);
}

bool LoadSettings(CynitaSettings& s, const std::string& folder) {
    std::string ini = folder + "\\cynita_settings.ini";
    std::ifstream fin(ini);
    if (!fin.is_open()) {
        // default: allocate some memory
        s.memory.assign(4096, 0);
        return false;
    }
    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto pos = line.find('=');
        if (pos == std::string::npos) continue;
        std::string key = line.substr(0, pos);
        std::string val = line.substr(pos + 1);
        if (key == "cpu_freq_mhz") s.cpu_freq_mhz = std::stoul(val);
        else if (key == "temperature") s.temperature = std::stof(val);
        else if (key == "fan_enabled") s.fan_enabled = (val == "1");
        else if (key == "auto_cooling") s.auto_cooling = (val == "1");
        else if (key == "ram_kb") s.ram_kb = std::stoi(val);
        else if (key == "rom_kb") s.rom_kb = std::stoi(val);
        else if (key == "ddr_kb") s.ddr_kb = std::stoi(val);
        else if (key == "sram_kb") s.sram_kb = std::stoi(val);
        else if (key == "dram_kb") s.dram_kb = std::stoi(val);
        else if (key == "cache_kb") s.cache_kb = std::stoi(val);
        else if (key == "uart_enabled") s.uart_enabled = (val == "1");
        else if (key == "spi_enabled") s.spi_enabled = (val == "1");
        else if (key == "i2c_enabled") s.i2c_enabled = (val == "1");
    }
    fin.close();

    // load memory binary if exists
    std::string memfile = folder + "\\cynita_memory.bin";
    std::ifstream mf(memfile, std::ios::binary);
    if (mf.is_open()) {
        mf.seekg(0, std::ios::end);
        size_t sz = (size_t)mf.tellg();
        mf.seekg(0, std::ios::beg);
        s.memory.resize(sz);
        if (sz) mf.read(reinterpret_cast<char*>(s.memory.data()), sz);
        mf.close();
    } else {
        // allocate based on ram size
        s.memory.assign(std::max(1, s.ram_kb) * 1024, 0);
    }
    return true;
}

bool SaveSettings(const CynitaSettings& s, const std::string& folder) {
    std::string ini = folder + "\\cynita_settings.ini";
    std::ofstream fout(ini, std::ios::trunc);
    if (!fout.is_open()) return false;
    fout << "cpu_freq_mhz=" << s.cpu_freq_mhz << "\n";
    fout << "temperature=" << s.temperature << "\n";
    fout << "fan_enabled=" << (s.fan_enabled ? "1" : "0") << "\n";
    fout << "auto_cooling=" << (s.auto_cooling ? "1" : "0") << "\n";
    fout << "ram_kb=" << s.ram_kb << "\n";
    fout << "rom_kb=" << s.rom_kb << "\n";
    fout << "ddr_kb=" << s.ddr_kb << "\n";
    fout << "sram_kb=" << s.sram_kb << "\n";
    fout << "dram_kb=" << s.dram_kb << "\n";
    fout << "cache_kb=" << s.cache_kb << "\n";
    fout << "uart_enabled=" << (s.uart_enabled ? "1" : "0") << "\n";
    fout << "spi_enabled=" << (s.spi_enabled ? "1" : "0") << "\n";
    fout << "i2c_enabled=" << (s.i2c_enabled ? "1" : "0") << "\n";
    fout.close();

    std::string memfile = folder + "\\cynita_memory.bin";
    std::ofstream mf(memfile, std::ios::binary | std::ios::trunc);
    if (!mf.is_open()) return false;
    if (!s.memory.empty()) mf.write(reinterpret_cast<const char*>(s.memory.data()), s.memory.size());
    mf.close();
    return true;
}

// -----------------------------------------------------------------------
// Main
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    // Setup window
    ImGui::CreateContext(); // temporary create to avoid wproc null
    ImGui::DestroyContext();

    // Register class
    WNDCLASSEX wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("CynitaLoader"), NULL };
    RegisterClassEx(&wc);
    HWND hwnd = CreateWindow(wc.lpszClassName, _T("Cynita Loader"), WS_OVERLAPPEDWINDOW, 100, 100, 900, 600, NULL, NULL, wc.hInstance, NULL);

    if (!CreateDeviceD3D(hwnd)) {
        CleanupDeviceD3D();
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // Load settings
    CynitaSettings settings;
    std::string folder = get_exe_folder();
    LoadSettings(settings, folder);

    // UI state
    bool show_demo_window = false;
    bool done = false;
    bool save_on_exit = false;
    bool cancel_no_save = false;
    int mem_page = 0;
    int edit_index = -1;
    int edit_value = 0;

    // For edge detection of F11
    bool prev_f11 = false;

    // Main loop
    MSG msg;
    while (!done) {
        while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT) done = true;
        }
        if (done) break;

        // Poll keyboard for F11 combos (Right Ctrl + F11 save; Left Ctrl + F11 no save)
        bool f11 = (GetAsyncKeyState(VK_F11) & 0x8000) != 0;
        if (f11 && !prev_f11) {
            bool rctrl = (GetAsyncKeyState(VK_RCONTROL) & 0x8000) != 0;
            bool lctrl = (GetAsyncKeyState(VK_LCONTROL) & 0x8000) != 0;
            if (rctrl) { save_on_exit = true; done = true; }
            else if (lctrl) { cancel_no_save = true; done = true; }
        }
        prev_f11 = f11;

        // Start ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // Main window (BIOS-like)
        ImGui::Begin("Cynita Loader - BIOS-like", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar);

        if (ImGui::BeginTabBar("CynitaTabs", ImGuiTabBarFlags_None)) {
            if (ImGui::BeginTabItem("Сведения")) {
                ImGui::Text("CPU");
                ImGui::Separator();
                ImGui::SliderInt("CPU frequency (MHz)", (int*)&settings.cpu_freq_mhz, 100, 5000);
                ImGui::SliderFloat("Temperature (C)", &settings.temperature, -20.0f, 150.0f);
                ImGui::Checkbox("Fan enabled", &settings.fan_enabled);
                ImGui::Checkbox("Auto cooling", &settings.auto_cooling);
                if (settings.auto_cooling && settings.temperature > 70.0f) {
                    ImGui::TextColored(ImVec4(1,0.4f,0.4f,1),"Warning: high temperature - throttling may occur");
                }
                ImGui::Spacing();
                ImGui::Text("Cooling");
                ImGui::Separator();
                static int fan_speed = 50;
                ImGui::SliderInt("Fan speed (%)", &fan_speed, 0, 100);
                ImGui::Text("Applied fan speed is simulated only.");
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Память")) {
                ImGui::Text("Memory configuration");
                ImGui::Separator();
                ImGui::InputInt("RAM (KB)", &settings.ram_kb);
                ImGui::InputInt("ROM (KB)", &settings.rom_kb);
                ImGui::InputInt("DDR (KB)", &settings.ddr_kb);
                ImGui::InputInt("SRAM (KB)", &settings.sram_kb);
                ImGui::InputInt("DRAM (KB)", &settings.dram_kb);
                ImGui::InputInt("Cache (KB)", &settings.cache_kb);

                if (ImGui::Button("Resize memory buffer to RAM size")) {
                    settings.memory.assign(std::max(1, settings.ram_kb) * 1024, 0);
                }
                ImGui::SameLine();
                if (ImGui::Button("Import memory...")) {
                    // Not implemented: open file dialog (platform-specific)
                }
                ImGui::SameLine();
                if (ImGui::Button("Export memory...")) {
                    std::string memfile = folder + "\\cynita_memory_export.bin";
                    std::ofstream mf(memfile, std::ios::binary | std::ios::trunc);
                    if (mf.is_open()) {
                        mf.write(reinterpret_cast<const char*>(settings.memory.data()), settings.memory.size());
                        mf.close();
                    }
                }

                ImGui::Separator();
                ImGui::Text("Memory viewer (first 1024 bytes)");
                const int bytes_per_row = 16;
                int rows = 64; // 64 * 16 = 1024 bytes
                ImGui::BeginChild("mem_view", ImVec2(0,300), true, ImGuiWindowFlags_HorizontalScrollbar);
                for (int r = 0; r < rows; ++r) {
                    int base = r * bytes_per_row;
                    ImGui::Text("%04X: ", base);
                    ImGui::SameLine();
                    for (int c = 0; c < bytes_per_row; ++c) {
                        int idx = base + c;
                        if (idx >= (int)settings.memory.size()) {
                            ImGui::Text(".. ");
                            ImGui::SameLine();
                            continue;
                        }
                        char label[16];
                        snprintf(label, sizeof(label), "%02X", settings.memory[idx]);
                        if (ImGui::SmallButton(label)) {
                            // start editing this byte (decimal)
                            edit_index = idx;
                            edit_value = settings.memory[idx];
                        }
                        ImGui::SameLine();
                    }
                    ImGui::NewLine();
                }
                ImGui::EndChild();

                if (edit_index >= 0) {
                    ImGui::Separator();
                    ImGui::Text("Edit byte at 0x%04X", edit_index);
                    ImGui::InputInt("Value (dec)", &edit_value);
                    if (ImGui::Button("Apply")) {
                        int v = edit_value & 0xFF;
                        settings.memory[edit_index] = static_cast<uint8_t>(v);
                        edit_index = -1;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Cancel")) {
                        edit_index = -1;
                    }
                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Другое")) {
                ImGui::Text("Peripherals");
                ImGui::Separator();
                ImGui::Checkbox("UART", &settings.uart_enabled);
                ImGui::Checkbox("SPI", &settings.spi_enabled);
                ImGui::Checkbox("I2C", &settings.i2c_enabled);
                ImGui::Spacing();
                ImGui::Text("Boot");
                ImGui::Separator();
                static int boot_order = 0; // 0 = Flash, 1 = Network, ...
                ImGui::Combo("Boot device", &boot_order, "Flash\0Network\0USB\0\0");
                if (ImGui::Button("Reset to defaults")) {
                    settings = CynitaSettings();
                    settings.memory.assign(4096, 0);
                }
                ImGui::Spacing();
                ImGui::TextWrapped("Зажмите Right Ctrl + F11, чтобы закрыть окно и сохранить все изменения или Left Ctrl + F11, чтобы закрыть окно без сохранения изменений.");
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        // Footer: save/cancel buttons
        ImGui::Separator();
        if (ImGui::Button("Save and Close (Right Ctrl + F11)")) {
            save_on_exit = true;
            done = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Close without Save (Left Ctrl + F11)")) {
            cancel_no_save = true;
            done = true;
        }

        ImGui::End(); // main window

        // Render
        ImGui::Render();
        const float clear_color_with_alpha[4] = { 0.06f, 0.06f, 0.06f, 1.0f };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        g_pSwapChain->Present(1, 0); // Present with vsync
    }

    // On exit: save if requested
    if (save_on_exit) {
        SaveSettings(settings, folder);
    } else if (!cancel_no_save) {
        // If exited by other means (window closed), do not save automatically.
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    CleanupDeviceD3D();
    UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

// -----------------------------------------------------------------------
// D3D helpers

bool CreateDeviceD3D(HWND hWnd) {
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
    HRESULT res = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags,
        featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK) return false;
    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D() {
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void CreateRenderTarget() {
    ID3D11Texture2D* pBackBuffer = NULL;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    if (pBackBuffer) {
        g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
        pBackBuffer->Release();
    }
}

void CleanupRenderTarget() {
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

// Win32 message handler
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;
    switch (msg) {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED) {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}