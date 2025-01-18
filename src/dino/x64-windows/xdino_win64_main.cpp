/// @file xdino_win64_main.cpp
/// @brief Implémentation de la fenêtre et des événements sur Windows.

// Expose le reste de notre programme.
#include <dino/xdino.h>
#include <dino/x64-windows/xdino_win64_rdr.h>

// Expose la bibliothèque standard.
#include <cmath>
#include <cstdio>
#include <random>

#include <Windows.h>
#include <Xinput.h>


// Déclaration des constantes.
constexpr int XDino_INIT_WIDTH = 640;
constexpr int XDino_INIT_HEIGHT = 480;

HINSTANCE gXDino_hInstance = nullptr;
HWND gXDino_hWindow = nullptr;
int64_t gXDino_tickStart = 0;
double gXDino_tickPeriod = 1.0;

std::random_device gXDino_randomDevice;
std::mt19937 gXDino_rng(gXDino_randomDevice());

// Déclaration des fonctions qui se trouvent plus bas dans le fichier.
void XDino_Win64_CreateWindow();
void XDino_Win64_CreateRenderer();
void XDino_Win64_DestroyWindow();
LRESULT CALLBACK XDino_Win64_HandleEvent(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Fonction d'entrée du programme, appelé par le système d'exploitation.
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    gXDino_hInstance = hInst;
    XDino_Win64_CreateWindow();
    ShowWindow(gXDino_hWindow, SW_SHOWNORMAL);
    UpdateWindow(gXDino_hWindow);

    // Puis on a la boucle principale d'événement, qui traite les messages envoyés par le système d'exploitation.
    MSG msg;
    BOOL fGotMessage;
    while ((fGotMessage = GetMessageW(&msg, nullptr, 0, 0))) {
        if (fGotMessage == -1)
            DINO_CRITICAL("GetMessageW failed");
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    Dino_GameShut();
    XDino_Win64_DestroyRenderer();
    XDino_Win64_DestroyWindow();

    return EXIT_SUCCESS;
}

#pragma region Window and events implementation

/// Crée la fenêtre graphique principale de rendu.
void XDino_Win64_CreateWindow()
{
    WNDCLASSEXW wcx{};
    wcx.cbSize = sizeof(WNDCLASSEXW);
    wcx.lpfnWndProc = &XDino_Win64_HandleEvent;
    wcx.hInstance = gXDino_hInstance;
    wcx.style = CS_OWNDC;
    wcx.lpszClassName = L"Dino";

    ATOM classWindow = RegisterClassExW(&wcx);
    if (classWindow == 0)
        DINO_CRITICAL("RegisterClassExW failed");

    RECT windowRect{0, 0, XDino_INIT_WIDTH, XDino_INIT_HEIGHT};
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
    int32_t width = windowRect.right - windowRect.left;
    int32_t height = windowRect.bottom - windowRect.top;

    CreateWindowExW(
        0,
        L"Dino",
        L"Dino",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width,
        height,
        nullptr,
        nullptr,
        gXDino_hInstance,
        nullptr
    );
    // gXDino_hWindow is set by XDino_Win64_HandleEvent, called immediately by CreateWindowExW.
    if (gXDino_hWindow == nullptr)
        DINO_CRITICAL("CreateWindowEx failed");
}

/// Gère les événements qu'envoie le système d'exploitation.
LRESULT CALLBACK XDino_Win64_HandleEvent(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // La fenêtre vient d'être construite.
    if (uMsg == WM_CREATE) {
        gXDino_hWindow = hWnd;

        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        gXDino_tickPeriod = static_cast<double>(freq.QuadPart);

        LARGE_INTEGER curTime;
        QueryPerformanceCounter(&curTime);
        gXDino_tickStart = curTime.QuadPart;

        RECT windowRect;
        GetClientRect(gXDino_hWindow, &windowRect);
        int32_t width = windowRect.right - windowRect.left;
        int32_t height = windowRect.bottom - windowRect.top;
        XDino_Win64_CreateRenderer(gXDino_hWindow, width, height);

        Dino_GameInit();

        return 0;
    }
    // L'utilisateur a demandé à détruire la fenêtre.
    if (uMsg == WM_DESTROY) {
        // On notifie le système d'exploitation qu'on veut arrêter. GetMessageW retournera FALSE.
        PostQuitMessage(0);
        return 0;
    }
    // L'utilisateur redimensionne la fenêtre.
    if (uMsg == WM_SIZE) {
        XDino_Win64_ResizeRenderer(LOWORD(lParam), HIWORD(lParam));
        return 0;
    }
    // Le système d'exploitation demande à redessiner la fenêtre.
    if (uMsg == WM_PAINT) {
        LARGE_INTEGER curTime;
        QueryPerformanceCounter(&curTime);
        int64_t tickCount = curTime.QuadPart - gXDino_tickStart;
        double timeSinceStart = static_cast<double>(tickCount) / gXDino_tickPeriod;
        XDino_Win64_BeginDraw();
        Dino_GameFrame(timeSinceStart);
        XDino_Win64_EndDraw();
        return 0;
    }
    // Délègue les autres événéments à l'implémentation par défaut du système d'exploitation.
    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

void XDino_Win64_DestroyWindow()
{
    gXDino_hWindow = nullptr;
}

void _impl_XDino_Critical(char const* pFunc, int line, char const* msg)
{
    char buffer[8192];
    snprintf(buffer, sizeof(buffer), "%s\n%s (line %d)", msg, pFunc, line);
    MessageBoxA(gXDino_hWindow, buffer, "Error", MB_SYSTEMMODAL | MB_SETFOREGROUND | MB_ICONERROR | MB_OK);
}

#pragma endregion

#pragma region Public API

bool XDino_GetGamepad(DinoGamepadIdx idx, DinoGamepad& outGamepad)
{
    outGamepad = {};
    if (idx == DinoGamepadIdx::Keyboard) {
        outGamepad.dpad_up = GetAsyncKeyState(VK_UP);
        outGamepad.dpad_left = GetAsyncKeyState(VK_LEFT);
        outGamepad.dpad_right = GetAsyncKeyState(VK_RIGHT);
        outGamepad.dpad_down = GetAsyncKeyState(VK_DOWN);
        outGamepad.btn_up = GetAsyncKeyState('Z') || GetAsyncKeyState('W');
        outGamepad.btn_left = GetAsyncKeyState('Q') || GetAsyncKeyState('A');
        outGamepad.btn_right = GetAsyncKeyState('D');
        outGamepad.btn_down = GetAsyncKeyState('S');
        outGamepad.start = GetAsyncKeyState(VK_RETURN) || GetAsyncKeyState(VK_SPACE);
        outGamepad.select = GetAsyncKeyState(VK_CONTROL) || GetAsyncKeyState(VK_SHIFT);
        outGamepad.shoulder_left = GetAsyncKeyState(VK_LBUTTON);
        outGamepad.shoulder_right = GetAsyncKeyState(VK_RBUTTON);

        float x, y, hypot;

        x = static_cast<float>(outGamepad.dpad_right) - static_cast<float>(outGamepad.dpad_left);
        y = static_cast<float>(outGamepad.dpad_down) - static_cast<float>(outGamepad.dpad_up);
        hypot = std::hypot(x, y);
        if (hypot > 0) {
            outGamepad.stick_left_x = x / hypot;
            outGamepad.stick_left_y = y / hypot;
        }

        x = static_cast<float>(outGamepad.btn_right) - static_cast<float>(outGamepad.btn_left);
        y = static_cast<float>(outGamepad.btn_down) - static_cast<float>(outGamepad.btn_up);
        hypot = std::hypot(x, y);
        if (hypot > 0) {
            outGamepad.stick_right_x = x / hypot;
            outGamepad.stick_right_y = y / hypot;
        }

        POINT p;
        if (GetCursorPos(&p) && ScreenToClient(gXDino_hWindow, &p)) {
            outGamepad.mouse_x = p.x;
            outGamepad.mouse_y = p.y;
        }

        return true;
    }

    int32_t idxNumber = static_cast<int32_t>(idx);
    if (idxNumber < 0 || idxNumber >= XUSER_MAX_COUNT)
        return false;

    XINPUT_STATE state;
    if (XInputGetState(idxNumber, &state) != ERROR_SUCCESS)
        return false;
    outGamepad.dpad_up = state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
    outGamepad.dpad_left = state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
    outGamepad.dpad_right = state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
    outGamepad.dpad_down = state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
    outGamepad.btn_up = state.Gamepad.wButtons & XINPUT_GAMEPAD_Y;
    outGamepad.btn_left = state.Gamepad.wButtons & XINPUT_GAMEPAD_X;
    outGamepad.btn_right = state.Gamepad.wButtons & XINPUT_GAMEPAD_B;
    outGamepad.btn_down = state.Gamepad.wButtons & XINPUT_GAMEPAD_A;
    outGamepad.start = state.Gamepad.wButtons & XINPUT_GAMEPAD_START;
    outGamepad.select = state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK;
    outGamepad.shoulder_left = state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
    outGamepad.shoulder_right = state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

    SHORT x, y;
    x = state.Gamepad.sThumbLX;
    y = state.Gamepad.sThumbLY;
    if (x <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || x >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
        outGamepad.stick_left_x = static_cast<float>(x) / 32768.f;
    if (y <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || y >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
        outGamepad.stick_left_y = static_cast<float>(y) / -32768.f;
    x = state.Gamepad.sThumbRX;
    y = state.Gamepad.sThumbRY;
    if (x <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || x >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
        outGamepad.stick_right_x = static_cast<float>(x) / 32768.f;
    if (y <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || y >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
        outGamepad.stick_right_y = static_cast<float>(y) / -32768.f;

    return true;
}

uint32_t XDino_RandomUint32(uint32_t min, uint32_t max)
{
    std::uniform_int_distribution<uint32_t> distribution(min, max);
    return distribution(gXDino_rng);
}

int32_t XDino_RandomInt32(int32_t min, int32_t max)
{
    std::uniform_int_distribution<int32_t> distribution(min, max);
    return distribution(gXDino_rng);
}

#pragma endregion