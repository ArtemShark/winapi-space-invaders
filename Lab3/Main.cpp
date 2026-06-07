#include <windows.h>
#include "Globals.h"
#include "GameHelpers.h"
#include "WindowProcs.h"
#include <commdlg.h>

using namespace Lab3;

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEXW wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = MainWndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszClassName = L"GameMainClass";
    if (!RegisterClassExW(&wc))
        return 0;

    WNDCLASSEXW wcPlayer = { 0 };
    wcPlayer.cbSize = sizeof(WNDCLASSEXW);
    wcPlayer.lpfnWndProc = PlayerWndProc;
    wcPlayer.hInstance = hInstance;
    wcPlayer.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wcPlayer.lpszClassName = L"GamePlayerClass";
    if (!RegisterClassExW(&wcPlayer))
        return 0;

    WNDCLASSEXW wcEnemy = { 0 };
    wcEnemy.cbSize = sizeof(WNDCLASSEXW);
    wcEnemy.lpfnWndProc = EnemyWndProc;
    wcEnemy.hInstance = hInstance;
    wcEnemy.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wcEnemy.lpszClassName = L"GameEnemyClass";
    if (!RegisterClassExW(&wcEnemy))
        return 0;

    WNDCLASSEXW wcBullet = { 0 };
    wcBullet.cbSize = sizeof(WNDCLASSEXW);
    wcBullet.lpfnWndProc = BulletWndProc;
    wcBullet.hInstance = hInstance;
    wcBullet.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wcBullet.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcBullet.lpszClassName = L"GameBulletClass";
    if (!RegisterClassExW(&wcBullet))
        return 0;

    g_hShipSprite = (HBITMAP)LoadImageW(NULL, L"ShipSprite.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (!g_hShipSprite) {
        MessageBoxW(NULL, L"Failed to load ShipSprite.bmp", L"Error", MB_ICONERROR);
        return 0;
    }
    g_hInvaderSprite = (HBITMAP)LoadImageW(NULL, L"InvaderSprite.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (!g_hInvaderSprite) {
        MessageBoxW(NULL, L"Failed to load InvaderSprite.bmp", L"Error", MB_ICONERROR);
        DeleteObject(g_hShipSprite);
        return 0;
    }

    ACCEL accelTable[] = {
        { FVIRTKEY, VK_F2,            ID_NEW_GAME },
        { FVIRTKEY | FCONTROL, '1',     ID_SIZE_SMALL },
        { FVIRTKEY | FCONTROL, '2',     ID_SIZE_MEDIUM },
        { FVIRTKEY | FCONTROL, '3',     ID_SIZE_LARGE },
        { FVIRTKEY | FCONTROL, 'C',     ID_BG_COLOR },
        { FVIRTKEY | FCONTROL, 'I',     ID_BG_IMAGE }
    };
    hAccelTable = CreateAcceleratorTable(accelTable, 6);

    HMENU hMenu = CreateMenu();
    HMENU hSubMenuGame = CreatePopupMenu();
    AppendMenuW(hSubMenuGame, MF_STRING, ID_NEW_GAME, L"New Game\tF2");
    AppendMenuW(hSubMenuGame, MF_STRING | MF_UNCHECKED, ID_SIZE_SMALL, L"Small\tCtrl+1");
    AppendMenuW(hSubMenuGame, MF_STRING | MF_CHECKED, ID_SIZE_MEDIUM, L"Medium\tCtrl+2");
    AppendMenuW(hSubMenuGame, MF_STRING | MF_UNCHECKED, ID_SIZE_LARGE, L"Large\tCtrl+3");
    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hSubMenuGame, L"Game");

    HMENU hMenuBg = CreatePopupMenu();
    AppendMenuW(hMenuBg, MF_STRING, ID_BG_COLOR, L"Solid Color\tCtrl+C");
    AppendMenuW(hMenuBg, MF_STRING, ID_BG_IMAGE, L"Load Image\tCtrl+I");

    HMENU hMenuBgMode = CreatePopupMenu();
    AppendMenuW(hMenuBgMode, MF_STRING | MF_UNCHECKED, ID_BG_MODE_CENTER, L"Center");
    AppendMenuW(hMenuBgMode, MF_STRING | MF_CHECKED, ID_BG_MODE_FILL, L"Fill");
    AppendMenuW(hMenuBgMode, MF_STRING | MF_UNCHECKED, ID_BG_MODE_TILE, L"Tile");
    AppendMenuW(hMenuBgMode, MF_STRING | MF_UNCHECKED, ID_BG_MODE_FIT, L"Fit");
    AppendMenuW(hMenuBg, MF_POPUP, (UINT_PTR)hMenuBgMode, L"Image Mode");
    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hMenuBg, L"Background");

    DWORD dwExStyle = WS_EX_LAYERED;
    DWORD dwStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;
    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);
    int posX = (screenW - g_mainWidth) / 2;
    int posY = (screenH - g_mainHeight) / 2;
    HWND hMainWnd = CreateWindowExW(
        dwExStyle,
        L"GameMainClass",
        L"Space Invaders",
        dwStyle,
        posX, posY,
        g_mainWidth, g_mainHeight,
        nullptr,
        hMenu,
        hInstance,
        nullptr
    );
    if (!hMainWnd)
        return 0;

    SetWindowPos(hMainWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    SetLayeredWindowAttributes(hMainWnd, 0, 255, LWA_ALPHA);
    ShowWindow(hMainWnd, nCmdShow);
    UpdateWindow(hMainWnd);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(hMainWnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    DeleteObject(g_hShipSprite);
    DeleteObject(g_hInvaderSprite);
    if (g_hBgImage)
        DeleteObject(g_hBgImage);

    return (int)msg.wParam;
}