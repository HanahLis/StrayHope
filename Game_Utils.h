#ifndef GAME_UTILS_H_
#define GAME_UTILS_H_

#include "Game_Base.h"
#include "Button.h"
#include "Enemy.h"
#include "Character.h"

bool Init();
bool LoadMedia();
void Close();

string GetHighScoreFromFile(string path);

void UpdateHighScore(string path, const int& score, const string& old_high_score);

int UpdateGameTimeAndScore(int& time, int& speed, int& score);

void RenderScrollingGround(int& speed, const int acceleration,
                            LTexture gGroundTexture, SDL_Renderer* gRenderer);

void HandlePlayButton(SDL_Event* e, Button& PlayButton,
                      bool& QuitMenu, bool& Play, Mix_Chunk* gClick);

void HandleHelpButton(SDL_Event* e, SDL_Rect(&gBackButton)[2],
    Button& HelpButton, Button& BackButton,
    LTexture gInstructionTexture, LTexture gBackButtonTexture,
    SDL_Renderer* gRenderer, bool& Quit_game, Mix_Chunk* gClick);

void HandleExitButton(SDL_Event* e, Button& ExitButton,
                    bool& Quit, Mix_Chunk* gClick);

void HandleContinueButton(Button ContinueButton, LTexture gContinueButtonTexture,
                        SDL_Event* e, SDL_Renderer* gRenderer, SDL_Rect(&gContinueButton)[2],
                        bool& Game_State, Mix_Chunk* gClick);

void HandlePauseButton(SDL_Event* e, SDL_Renderer* gRenderer, SDL_Rect(&gContinueButton)[2],
                    Button& PauseButton, Button ContinueButton,
                    LTexture gContinueButtonTexture,
                    bool& game_state, Mix_Chunk* gClick);

void GenerateEnemy(Enemy& enemy1, Enemy& enemy2, Enemy& enemy3,
    SDL_Rect(&gEnemyClips)[FLYING_FRAMES], SDL_Renderer* gRenderer);

bool CheckColission(Character character, SDL_Rect* char_clip,
                    Enemy enemy, SDL_Rect* enemy_clip = nullptr);

bool CheckEnemyColission(Character character, Enemy enemy1, Enemy enemy2, Enemy enemy3,
    SDL_Rect* char_clip, SDL_Rect* enemy_clip = nullptr);

void ControlCharFrame(int& frame);

void ControlEnemyFrame(int& frame);

void DrawPlayerScore(LTexture gTextTexture, LTexture gScoreTexture,
                    SDL_Color textColor, SDL_Renderer* gRenderer,
                    TTF_Font* gFont, const int& score);

void DrawPlayerHighScore(LTexture gTextTexture, LTexture gHighScoreTexture,
    SDL_Color textColor, SDL_Renderer* gRenderer,
    TTF_Font* gFont, const string& HighScore);

void DrawEndGameSelection(LTexture gLoseTexture,
    SDL_Event* e, SDL_Renderer* gRenderer,
    bool& Play_Again);

#endif
