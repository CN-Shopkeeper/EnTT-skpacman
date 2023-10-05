#pragma once

#include "pch.hpp"
#include "utils/vector2.hpp"

constexpr bool DebugMode = true;

constexpr SDL_Color KeyColor = {118, 66, 138, 255};
constexpr SDL_Color BlinkyColor = {255, 0, 0, 255};
constexpr SDL_Color PinkyColor = {255, 192, 203, 255};
constexpr SDL_Color InkyColor = {0, 255, 255, 255};
constexpr SDL_Color ClydeColor = {255, 127, 80, 255};
constexpr SDL_Color ScaredColor = {0, 0, 255, 255};
constexpr SDL_Color InvincibleColor = {255, 0, 0, 255};
constexpr SDL_Color WhiteColor = {255, 255, 255, 255};

// tilesheet

inline std::string TilesheetName = "tilesheet";
constexpr int TilesheetCol = 7;
constexpr int TilesheetRow = 1;

enum class ImageTileType {
  Wall = 0,
  Pacman,
  PacmanEat,
  Bean,
  PowerBean,
  Ghost,
  GhostDoor,
};

// 界面相关

constexpr size_t tetrisHeight = 9, tetrisWidth = 5;

constexpr int MapWidth = tetrisWidth * 3 * 2;
constexpr int MapHeight = tetrisHeight * 3 + 2;

constexpr size_t TileSize = 16;

constexpr size_t WindowHeight = TileSize * MapHeight;
constexpr size_t WindowWidth = TileSize * MapWidth;

constexpr Coor PacmanInitCoor = {MapWidth / 2 - 1, 7 * 3 + 1};

constexpr Coor GhostInitCoor = {MapWidth / 2 - 2, 4 * 3 + 1};

// 游戏机制相关

constexpr float ghostScaredTime = 15.0f;
constexpr int BeanScore = 10;
constexpr int MultiKillReward = 200;
constexpr int TimeBonusPerSec = 300;
constexpr int RemainingLifeBonus = 1000;
constexpr int BonusTimeCount = 35;
constexpr int RemainingLifeCount = 2;
constexpr float InvincibleTime = 3.0f;

constexpr int Framerate = 33;
constexpr float FrameTime = 1000.f / Framerate;

constexpr Coor outsideHouseRange[2] = {{11, 11}, {18, 15}};
constexpr Coor insideHouseRange[2] = {{12, 12}, {17, 14}};
