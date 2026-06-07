#include "WindowProcs.h"
#include "Globals.h"
#include "GameHelpers.h"
#include <algorithm>
#include <limits>
#include <commdlg.h>

namespace Lab3 {

	LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_CREATE:
		{
			g_Player = CreateWindowExW(
				0,
				L"GamePlayerClass",
				nullptr,
				WS_CHILD | WS_VISIBLE,
				g_playerX, g_playerY,
				PLAYER_WIDTH, PLAYER_HEIGHT,
				hWnd,
				nullptr,
				nullptr,
				nullptr
			);
			InitializeEnemies(hWnd);
			SetTimer(hWnd, TIMER_ENEMY, 20, nullptr);
			SetTimer(hWnd, TIMER_ANIM, 100, nullptr);
			SetTimer(hWnd, TIMER_BULLET, 50, nullptr);
		}
		break;

		case WM_TIMER:
		{
			if (wParam == TIMER_ENEMY) {
				const int boundaryOffset = 40;
				LONG minX = std::numeric_limits<LONG>::max();
				LONG maxX = 0;
				for (HWND hEnemy : g_Enemies) {
					RECT r;
					GetWindowRect(hEnemy, &r);
					MapWindowPoints(HWND_DESKTOP, hWnd, (LPPOINT)&r, 2);
					minX = std::min<LONG>(minX, r.left);
					maxX = std::max<LONG>(maxX, r.left);
				}
				if (minX <= LEFT_MARGIN + boundaryOffset) g_enemyDir = 1;
				if (maxX >= (g_mainWidth - RIGHT_MARGIN - ENEMY_WIDTH - boundaryOffset)) g_enemyDir = -1;

				for (HWND hEnemy : g_Enemies) {
					RECT r;
					GetWindowRect(hEnemy, &r);
					MapWindowPoints(HWND_DESKTOP, hWnd, (LPPOINT)&r, 2);
					int newX = r.left + (g_enemyDir * 2);
					MoveWindow(hEnemy, newX, r.top, ENEMY_WIDTH, ENEMY_HEIGHT, TRUE);
				}
			}
			else if (wParam == TIMER_ANIM) {
				g_playerFrame = (g_playerFrame + 1) % NUM_PLAYER_FRAMES;
				g_enemyFrame = (g_enemyFrame + 1) % NUM_ENEMY_FRAMES;
				InvalidateRect(g_Player, nullptr, TRUE);
				for (HWND hEnemy : g_Enemies)
					InvalidateRect(hEnemy, nullptr, TRUE);
			}
			else if (wParam == TIMER_BULLET) {
				for (size_t i = 0; i < g_Bullets.size(); )
				{
					Bullet& b = g_Bullets[i];

					b.y -= BULLET_SPEED;
					if (b.y + BULLET_HEIGHT < 0) {
						DestroyWindow(b.hwnd);
						g_Bullets.erase(g_Bullets.begin() + i);
						continue;
					}
					else {
						MoveWindow(b.hwnd, b.x, b.y, BULLET_WIDTH, BULLET_HEIGHT, TRUE);
					}

					RECT bulletRect;
					GetWindowRect(b.hwnd, &bulletRect);
					MapWindowPoints(HWND_DESKTOP, hWnd, (LPPOINT)&bulletRect, 2);

					bool collisionDetected = false;
					for (size_t j = 0; j < g_Enemies.size(); j++) {
						HWND hEnemy = g_Enemies[j];
						RECT enemyRect;
						GetWindowRect(hEnemy, &enemyRect);
						MapWindowPoints(HWND_DESKTOP, hWnd, (LPPOINT)&enemyRect, 2);

						RECT tmp;
						if (IntersectRect(&tmp, &bulletRect, &enemyRect)) {
							g_score += 10;
							DestroyWindow(hEnemy);
							g_Enemies.erase(g_Enemies.begin() + j);

							collisionDetected = true;
							break;
						}
					}
					if (collisionDetected) {
						DestroyWindow(b.hwnd);
						g_Bullets.erase(g_Bullets.begin() + i);
						InvalidateRect(hWnd, nullptr, FALSE);
						continue;
					}
					i++;
				}
			}
		}
		break;

		case WM_KEYDOWN:
		{
			switch (wParam) {
			case VK_LEFT:
			{
				int newX = g_playerX - PLAYER_MOVE_STEP;
				if (newX < LEFT_MARGIN)
					newX = LEFT_MARGIN;
				g_playerX = newX;
				MoveWindow(g_Player, g_playerX, g_playerY, PLAYER_WIDTH, PLAYER_HEIGHT, TRUE);
			}
			break;
			case VK_RIGHT:
			{
				int maxX = g_mainWidth - RIGHT_MARGIN - PLAYER_WIDTH;
				int newX = g_playerX + PLAYER_MOVE_STEP;
				if (newX > maxX)
					newX = maxX;
				g_playerX = newX;
				MoveWindow(g_Player, g_playerX, g_playerY, PLAYER_WIDTH, PLAYER_HEIGHT, TRUE);
			}
			break;
			case VK_SPACE:
			{
				int bulletX = g_playerX + (PLAYER_WIDTH - BULLET_WIDTH) / 2;
				int bulletY = g_playerY - BULLET_HEIGHT;
				HWND hBullet = CreateWindowExW(
					0,
					L"GameBulletClass",
					nullptr,
					WS_CHILD | WS_VISIBLE,
					bulletX, bulletY,
					BULLET_WIDTH, BULLET_HEIGHT,
					hWnd,
					nullptr,
					nullptr,
					nullptr
				);
				if (hBullet) {
					Bullet newBullet = { hBullet, bulletX, bulletY };
					g_Bullets.push_back(newBullet);
				}
			}
			break;
			default:
				break;
			}
		}
		break;
		case WM_COMMAND:
		{
			int cmd = LOWORD(wParam);
			switch (cmd) {
			case ID_NEW_GAME:
				NewGameReset(hWnd);
				MessageBoxW(hWnd, L"New Game Started! Game state has been reset.", L"Info", MB_OK);
				break;
			case ID_SIZE_SMALL:
				g_gameSize = SIZE_SMALL;
				CheckMenuItem(GetMenu(hWnd), ID_SIZE_SMALL, MF_BYCOMMAND | MF_CHECKED);
				CheckMenuItem(GetMenu(hWnd), ID_SIZE_MEDIUM, MF_BYCOMMAND | MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd), ID_SIZE_LARGE, MF_BYCOMMAND | MF_UNCHECKED);
				UpdateGameSize(hWnd, SIZE_SMALL);
				break;
			case ID_SIZE_MEDIUM:
				g_gameSize = SIZE_MEDIUM;
				CheckMenuItem(GetMenu(hWnd), ID_SIZE_SMALL, MF_BYCOMMAND | MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd), ID_SIZE_MEDIUM, MF_BYCOMMAND | MF_CHECKED);
				CheckMenuItem(GetMenu(hWnd), ID_SIZE_LARGE, MF_BYCOMMAND | MF_UNCHECKED);
				UpdateGameSize(hWnd, SIZE_MEDIUM);
				break;
			case ID_SIZE_LARGE:
				g_gameSize = SIZE_LARGE;
				CheckMenuItem(GetMenu(hWnd), ID_SIZE_SMALL, MF_BYCOMMAND | MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd), ID_SIZE_MEDIUM, MF_BYCOMMAND | MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd), ID_SIZE_LARGE, MF_BYCOMMAND | MF_CHECKED);
				UpdateGameSize(hWnd, SIZE_LARGE);
				break;
			case ID_BG_COLOR:
			{
				CHOOSECOLOR cc;
				COLORREF acrCustClr[16] = { 0 };
				ZeroMemory(&cc, sizeof(cc));
				cc.lStructSize = sizeof(cc);
				cc.hwndOwner = hWnd;
				cc.lpCustColors = acrCustClr;
				cc.rgbResult = g_bgColor;
				cc.Flags = CC_FULLOPEN | CC_RGBINIT;
				if (ChooseColor(&cc)) {
					g_bgColor = cc.rgbResult;
					g_bgMode = BG_SOLID;
					InvalidateRect(hWnd, nullptr, TRUE);
				}
			}
			break;
			case ID_BG_IMAGE:
			{
				OPENFILENAME ofn;
				WCHAR szFileName[MAX_PATH] = L"";
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hWnd;
				ofn.lpstrFilter = L"Bitmap Files (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0";
				ofn.lpstrFile = szFileName;
				ofn.nMaxFile = MAX_PATH;
				ofn.Flags = OFN_FILEMUSTEXIST;
				if (GetOpenFileName(&ofn)) {
					HBITMAP hNewBg = (HBITMAP)LoadImageW(NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
					if (hNewBg) {
						if (g_hBgImage)
							DeleteObject(g_hBgImage);
						g_hBgImage = hNewBg;
						g_bgMode = BG_IMAGE;
						InvalidateRect(hWnd, nullptr, TRUE);
					}
					else {
						DWORD err = GetLastError();
						MessageBoxW(hWnd, L"Failed to load background image", L"Error", MB_ICONERROR);
					}
				}
			}
			break;
			case ID_BG_MODE_CENTER:
				g_bgImageMode = IMG_MODE_CENTER;
				CheckMenuItem(GetMenu(hWnd), ID_BG_MODE_CENTER, MF_BYCOMMAND | MF_CHECKED);
				CheckMenuItem(GetMenu(hWnd), ID_BG_MODE_FILL, MF_BYCOMMAND | MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd), ID_BG_MODE_TILE, MF_BYCOMMAND | MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd), ID_BG_MODE_FIT, MF_BYCOMMAND | MF_UNCHECKED);
				InvalidateRect(hWnd, nullptr, TRUE);
				break;
			case ID_BG_MODE_FILL:
				g_bgImageMode = IMG_MODE_FILL;
				CheckMenuItem(GetMenu(hWnd), ID_BG_MODE_CENTER, MF_BYCOMMAND | MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd), ID_BG_MODE_FILL, MF_BYCOMMAND | MF_CHECKED);
				CheckMenuItem(GetMenu(hWnd), ID_BG_MODE_TILE, MF_BYCOMMAND | MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd), ID_BG_MODE_FIT, MF_BYCOMMAND | MF_UNCHECKED);
				InvalidateRect(hWnd, nullptr, TRUE);
				break;
			case ID_BG_MODE_TILE:
				g_bgImageMode = IMG_MODE_TILE;
				CheckMenuItem(GetMenu(hWnd), ID_BG_MODE_CENTER, MF_BYCOMMAND | MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd), ID_BG_MODE_FILL, MF_BYCOMMAND | MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd), ID_BG_MODE_TILE, MF_BYCOMMAND | MF_CHECKED);
				CheckMenuItem(GetMenu(hWnd), ID_BG_MODE_FIT, MF_BYCOMMAND | MF_UNCHECKED);
				InvalidateRect(hWnd, nullptr, TRUE);
				break;
			case ID_BG_MODE_FIT:
				g_bgImageMode = IMG_MODE_FIT;
				CheckMenuItem(GetMenu(hWnd), ID_BG_MODE_CENTER, MF_BYCOMMAND | MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd), ID_BG_MODE_FILL, MF_BYCOMMAND | MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd), ID_BG_MODE_TILE, MF_BYCOMMAND | MF_UNCHECKED);
				CheckMenuItem(GetMenu(hWnd), ID_BG_MODE_FIT, MF_BYCOMMAND | MF_CHECKED);
				InvalidateRect(hWnd, nullptr, TRUE);
				break;
			default:
				break;
			}
		}
		break;

		case WM_ERASEBKGND:
		{
			HDC hdc = (HDC)wParam;
			RECT rc;
			GetClientRect(hWnd, &rc);
			if (g_bgMode == BG_IMAGE && g_hBgImage) {
				HDC memDC = CreateCompatibleDC(hdc);
				HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, g_hBgImage);
				BITMAP bm;
				GetObject(g_hBgImage, sizeof(BITMAP), &bm);
				int imgW = bm.bmWidth;
				int imgH = bm.bmHeight;
				switch (g_bgImageMode) {
				case IMG_MODE_FILL:
					StretchBlt(hdc, 0, 0, rc.right, rc.bottom, memDC, 0, 0, imgW, imgH, SRCCOPY);
					break;
				case IMG_MODE_CENTER:
				{
					int x = (rc.right - imgW) / 2;
					int y = (rc.bottom - imgH) / 2;
					BitBlt(hdc, x, y, imgW, imgH, memDC, 0, 0, SRCCOPY);
					break;
				}
				case IMG_MODE_TILE:
				{
					for (int y = 0; y < rc.bottom; y += imgH) {
						for (int x = 0; x < rc.right; x += imgW) {
							BitBlt(hdc, x, y, imgW, imgH, memDC, 0, 0, SRCCOPY);
						}
					}
					break;
				}
				case IMG_MODE_FIT:
				{
					double scale = std::max((double)rc.right / imgW, (double)rc.bottom / imgH);
					int newW = (int)(imgW * scale);
					int newH = (int)(imgH * scale);
					int x = (rc.right - newW) / 2;
					int y = (rc.bottom - newH) / 2;
					SetStretchBltMode(hdc, HALFTONE);
					SetBrushOrgEx(hdc, 0, 0, NULL);
					StretchBlt(hdc, x, y, newW, newH, memDC, 0, 0, imgW, imgH, SRCCOPY);
					break;
				}
				}
				SelectObject(memDC, oldBmp);
				DeleteDC(memDC);
			}
			else {
				HBRUSH brush = CreateSolidBrush(g_bgColor);
				FillRect(hdc, &rc, brush);
				DeleteObject(brush);
			}
			return 1;
		}

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, RGB(0, 0, 0));
			wchar_t scoreText[32];
			swprintf_s(scoreText, L"Score: %d", g_score);
			RECT rcClient;
			GetClientRect(hWnd, &rcClient);
			int textX = 10;
			int textY = rcClient.bottom - 30; 
			TextOutW(hdc, textX, textY, scoreText, (int)wcslen(scoreText));
			EndPaint(hWnd, &ps);
			return 0;
		}

		case WM_ACTIVATEAPP:
		{
			if (wParam)
				SetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA);
			else
				SetLayeredWindowAttributes(hWnd, 0, 105, LWA_ALPHA);
		}
		break;

		case WM_DESTROY:
			KillTimer(hWnd, TIMER_ENEMY);
			KillTimer(hWnd, TIMER_ANIM);
			KillTimer(hWnd, TIMER_BULLET);
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProcW(hWnd, message, wParam, lParam);
		}
		return 0;
	}

	LRESULT CALLBACK PlayerWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			HDC memDC = CreateCompatibleDC(hdc);
			HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, g_hShipSprite);
			int sx = g_playerFrame * PLAYER_FRAME_WIDTH;
			int sy = 0;
			BitBlt(hdc, 0, 0, PLAYER_WIDTH, PLAYER_HEIGHT, memDC, sx, sy, SRCCOPY);
			SelectObject(memDC, oldBmp);
			DeleteDC(memDC);
			EndPaint(hWnd, &ps);
		}
		break;
		default:
			return DefWindowProcW(hWnd, message, wParam, lParam);
		}
		return 0;
	}

	LRESULT CALLBACK EnemyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			HDC memDC = CreateCompatibleDC(hdc);
			HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, g_hInvaderSprite);
			int sx = g_enemyFrame * ENEMY_FRAME_WIDTH;
			int sy = 0;
			BitBlt(hdc, 0, 0, ENEMY_WIDTH, ENEMY_HEIGHT, memDC, sx, sy, SRCCOPY);
			SelectObject(memDC, oldBmp);
			DeleteDC(memDC);
			EndPaint(hWnd, &ps);
		}
		break;
		default:
			return DefWindowProcW(hWnd, message, wParam, lParam);
		}
		return 0;
	}

	LRESULT CALLBACK BulletWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			RECT rc;
			GetClientRect(hWnd, &rc);
			FillRect(hdc, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));
			EndPaint(hWnd, &ps);
		}
		break;
		default:
			return DefWindowProcW(hWnd, message, wParam, lParam);
		}
		return 0;
	}
}