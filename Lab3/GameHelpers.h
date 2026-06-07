#pragma once
#include <windows.h>
#include "Globals.h"

namespace Lab3 {
	void UpdateGameSize(HWND hWnd, GameSize size);
	void InitializeEnemies(HWND hWnd);
	void NewGameReset(HWND hWnd);
}