# WinAPI Space Invaders

A Space Invaders-style arcade game built entirely with the Win32 API in C++. Part of the "Programming in Graphical Environment" course at Warsaw University of Technology.

## Gameplay

Control a ship at the bottom of the screen, shoot bullets upward to destroy rows of invaders before they reach you. The game uses bitmap sprites for the ship and invaders.

### Features

- Pure Win32 API — no game engine or framework, just `CreateWindow`, `WndProc`, and GDI
- Separate window classes for player, enemies, and bullets
- Bitmap sprite rendering with `LoadImage` and `BitBlt`
- Configurable game grid sizes
- Keyboard input handling via `WM_KEYDOWN`

## Tech

C++, Win32 API, GDI, bitmap sprites
