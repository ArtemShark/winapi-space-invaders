#pragma once
#include <windows.h>
#include <vector>

namespace Lab3 {
	struct Bullet {
		HWND hwnd;
		int x;
		int y;
	};

	extern std::vector<Bullet> g_Bullets;

	struct GameDimensions {
		int width;
		int height;
	};

	extern const GameDimensions SMALL;
	extern const GameDimensions MEDIUM;
	extern const GameDimensions LARGE;

	enum GameSize { SIZE_SMALL, SIZE_MEDIUM, SIZE_LARGE };
	extern GameSize g_gameSize;

	extern int g_mainWidth;
	extern int g_mainHeight;

	extern const int PLAYER_WIDTH;
	extern const int PLAYER_HEIGHT;
	extern const int ENEMY_WIDTH;
	extern const int ENEMY_HEIGHT;
	extern const int LEFT_MARGIN;
	extern const int RIGHT_MARGIN;

	extern const int PLAYER_MOVE_STEP;
	extern const int ENEMY_MOVE;
	extern const int ENEMY_MOVE_MAX;
	extern const int BULLET_SPEED;
	extern const int BULLET_WIDTH;
	extern const int BULLET_HEIGHT;

    #define PLAYER_FRAME_WIDTH   50
    #define PLAYER_FRAME_HEIGHT  50
    #define ENEMY_FRAME_WIDTH    50
    #define ENEMY_FRAME_HEIGHT   40

	extern const int NUM_PLAYER_FRAMES;
	extern const int NUM_ENEMY_FRAMES;

    #define TIMER_ENEMY  1
    #define TIMER_ANIM   3
    #define TIMER_BULLET 4

    #define ID_NEW_GAME         1001
    #define ID_SIZE_SMALL       1002
    #define ID_SIZE_MEDIUM      1003
    #define ID_SIZE_LARGE       1004
    #define ID_BG_COLOR         1005  
    #define ID_BG_IMAGE         1006  

    #define ID_BG_MODE_CENTER   1010
    #define ID_BG_MODE_FILL     1011
    #define ID_BG_MODE_TILE     1012
    #define ID_BG_MODE_FIT      1013

	extern int g_playerX;
	extern int g_playerY;
	extern int g_enemyX;
	extern int g_enemyY;
	extern int g_enemyDir;

	extern int g_playerFrame;
	extern int g_enemyFrame;

	extern int g_score;

	extern HBITMAP g_hShipSprite; 
	extern HBITMAP g_hInvaderSprite; 

	extern HWND g_Player;
	extern std::vector<HWND> g_Enemies;

	extern HACCEL hAccelTable;

	enum BackgroundMode { BG_SOLID, BG_IMAGE };
	extern BackgroundMode g_bgMode;
	extern COLORREF       g_bgColor; 
	extern HBITMAP        g_hBgImage;

	enum BgImageDisplayMode { IMG_MODE_CENTER, IMG_MODE_FILL, IMG_MODE_TILE, IMG_MODE_FIT };
	extern BgImageDisplayMode g_bgImageMode;
}