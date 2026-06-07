#include "GameHelpers.h"
#include "Globals.h"
#include <algorithm>
#include <limits>

namespace Lab3 
{
	void UpdateGameSize(HWND hWnd, GameSize size)
	{
		GameDimensions dims;
		switch (size) {
		case SIZE_SMALL:  dims = SMALL;  break;
		case SIZE_MEDIUM: dims = MEDIUM; break;
		case SIZE_LARGE:  dims = LARGE;  break;
		}
		g_mainWidth = dims.width;
		g_mainHeight = dims.height;
		SetWindowPos(hWnd, nullptr, 0, 0, g_mainWidth, g_mainHeight, SWP_NOMOVE | SWP_NOZORDER);

		g_playerY = g_mainHeight - PLAYER_HEIGHT - 50;
		g_playerX = (g_mainWidth - PLAYER_WIDTH) / 2;

		if (g_Player)
			MoveWindow(g_Player, g_playerX, g_playerY, PLAYER_WIDTH, PLAYER_HEIGHT, TRUE);

		InvalidateRect(hWnd, nullptr, TRUE);
	}

	void InitializeEnemies(HWND hWnd)
	{
		for (HWND hEnemy : g_Enemies)
			DestroyWindow(hEnemy);
		g_Enemies.clear();

		const int numRows = 3;
		const int numCols = 5;
		int gap = 10;
		int totalWidth = numCols * ENEMY_WIDTH + (numCols - 1) * gap;
		int startX = (g_mainWidth - totalWidth) / 2;
		int startY = 50; 

		for (int row = 0; row < numRows; ++row) {
			for (int col = 0; col < numCols; ++col) {
				int ex = startX + col * (ENEMY_WIDTH + gap);
				int ey = startY + row * (ENEMY_HEIGHT + gap);
				HWND hEnemy = CreateWindowExW(
					0,
					L"GameEnemyClass",
					nullptr,
					WS_CHILD | WS_VISIBLE,
					ex, ey,
					ENEMY_WIDTH, ENEMY_HEIGHT,
					hWnd,
					nullptr,
					nullptr,
					nullptr
				);
				if (hEnemy)
					g_Enemies.push_back(hEnemy);
			}
		}
	}

	void NewGameReset(HWND hWnd)
	{
		g_bgMode = BG_SOLID;
		g_bgColor = RGB(255, 255, 255);
		if (g_hBgImage)
		{
			DeleteObject(g_hBgImage);
			g_hBgImage = NULL;
		}

		g_score = 0;

		UpdateGameSize(hWnd, g_gameSize);

		g_enemyDir = 1;

		for (size_t i = 0; i < g_Bullets.size(); ++i)
		{
			DestroyWindow(g_Bullets[i].hwnd);
		}
		g_Bullets.clear();

		InitializeEnemies(hWnd);

		InvalidateRect(hWnd, nullptr, TRUE);
	}
}