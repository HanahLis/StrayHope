#ifndef GAME_BASE_
#define GAME_BASE_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
using namespace std;

#define TIME_MAX 1000
#define GROUND 430
#define MAX_HEIGHT 250

#define BASE_OFFSET_SPEED 0

#define SPEED_INCREASEMENT 2
#define SCORE_INCREASEMENT 1
#define TIME_INCREASEMENT 1
#define FRAME_INCREASEMENT 1

#define GROUND_SPEED 5
#define ENEMY_SPEED 5
#define MAX_ENEMY_WIDTH 100

#define IN_AIR_ENEMY 1
#define ON_GROUND_ENEMY 0
#define SLOW_FRAME_CHAR 4
#define SLOW_FRAME_ENEMY 4

#define SMALL_BUTTON 1
#define COMMON_BUTTON 2

const string WINDOW_TITLE = "HANNAH LIKE DINO";

const int SCREEN_WIDTH = 928;
const int SCREEN_HEIGHT = 522;

const int RUNNING_FRAMES = 6;
const int FLYING_FRAMES = 5;

enum ButtonSprite
{
	BUTTON_MOUSE_OUT = 0,
	BUTTON_MOUSE_OVER = 1,
//	BUTTON_TOTAL = 2
};

#endif
