#include "Globals.h"
#include <limits.h>

namespace Lab3 {

    std::vector<Bullet> g_Bullets;

    const GameDimensions SMALL = { 600, 400 };
    const GameDimensions MEDIUM = { 800, 600 };
    const GameDimensions LARGE = { 1024, 768 };

    GameSize g_gameSize = SIZE_MEDIUM;

    int g_mainWidth = MEDIUM.width;
    int g_mainHeight = MEDIUM.height;

    const int PLAYER_WIDTH = 50;
    const int PLAYER_HEIGHT = 50;
    const int ENEMY_WIDTH = 50;
    const int ENEMY_HEIGHT = 40;
    const int LEFT_MARGIN = 10;
    const int RIGHT_MARGIN = 10;

    const int PLAYER_MOVE_STEP = 10;
    const int ENEMY_MOVE = 25;
    const int ENEMY_MOVE_MAX = 50;
    const int BULLET_SPEED = 15;
    const int BULLET_WIDTH = 5;
    const int BULLET_HEIGHT = 15;


    const int NUM_PLAYER_FRAMES = 4;
    const int NUM_ENEMY_FRAMES = 4;

    int g_playerX = 370;
    int g_playerY = 500;
    int g_enemyX = 370;
    int g_enemyY = 50;
    int g_enemyDir = 1;

    int g_playerFrame = 0;
    int g_enemyFrame = 0;

    int g_score = 0;

    HBITMAP g_hShipSprite = NULL; 
    HBITMAP g_hInvaderSprite = NULL; 

    HWND g_Player = nullptr;
    std::vector<HWND> g_Enemies;

    HACCEL hAccelTable = NULL;

    BackgroundMode g_bgMode = BG_SOLID;
    COLORREF       g_bgColor = RGB(255, 255, 255); 
    HBITMAP        g_hBgImage = NULL;

    BgImageDisplayMode g_bgImageMode = IMG_MODE_FILL;
}