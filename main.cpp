#include "Game_Base.h"
#include "Game_Utils.h"
#include "LTexture.h"
#include "Button.h"
#include "Character.h"
#include "Enemy.h"

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Color textColor = { 255, 255, 255 };
TTF_Font* gFont = nullptr;
Mix_Music* gMusic = nullptr;
Mix_Music* gMenuMusic = nullptr;
Mix_Chunk* gClick = nullptr;
Mix_Chunk* gJump = nullptr;
Mix_Chunk* gLose = nullptr;

SDL_Rect gPlayButton;
SDL_Rect gHelpButton;
SDL_Rect gExitButton;
SDL_Rect gBackButton[2];
SDL_Rect gPauseButton[2];
SDL_Rect gContinueButton[2];
SDL_Rect gPlayAgainButton[2];
SDL_Rect gCharacterClips[RUNNING_FRAMES];
SDL_Rect gEnemyClips[FLYING_FRAMES];

LTexture gMenuTexture;
LTexture gInstructionTexture;
LTexture gBackgroundTexture;
LTexture gCharacterTexture;
LTexture gGroundTexture;
LTexture gPlayButtonTexture;
LTexture gHelpButtonTexture;
LTexture gExitButtonTexture;
LTexture gBackButtonTexture;
LTexture gPauseButtonTexture;
LTexture gContinueButtonTexture;
LTexture gLoseTexture;
LTexture gText1Texture;
LTexture gScoreTexture;
LTexture gText2Texture;
LTexture gHighScoreTexture;

Button PlayButton(389, 186);
Button HelpButton(389, 293);
Button ExitButton(389, 402);
Button BackButton(744, 427);
Button PauseButton(31, 29);
Button ContinueButton(31, 29);

Character character;

int main(int argc, char* argv[])
{
	if (!Init())    cout<<"Failed to initialize!"<<endl;
	else
	{
		if (!LoadMedia())   cout<<"Failed to load media!"<<endl;
		else
		{
			bool Quit_Menu = false;
			bool Play_Again = false;

			Mix_PlayMusic(gMenuMusic, -1);
			while (!Quit_Menu)
			{
				SDL_Event e_mouse;
				while (SDL_PollEvent(&e_mouse) != 0)
				{
					if (e_mouse.type == SDL_QUIT)
					{
						Quit_Menu = true;
					}

					bool Quit_Game = false;
					HandlePlayButton(&e_mouse, PlayButton, Quit_Menu, Play_Again, gClick);

					HandleHelpButton(&e_mouse, gBackButton, HelpButton, BackButton,
									 gInstructionTexture, gBackButtonTexture,
									 gRenderer, Quit_Game, gClick);

					HandleExitButton(&e_mouse, ExitButton, Quit_Menu, gClick);

					if (Quit_Game == true) return 0;
				}

				gMenuTexture.Render(0, 0, gRenderer);

				SDL_Rect* currentClip_Play = &gPlayButton;
				PlayButton.Render(currentClip_Play, gRenderer, gPlayButtonTexture);

				SDL_Rect* currentClip_Help = &gHelpButton;
				HelpButton.Render(currentClip_Help, gRenderer, gHelpButtonTexture);

				SDL_Rect* currentClip_Exit = &gExitButton;
				ExitButton.Render(currentClip_Exit, gRenderer, gExitButtonTexture);

				SDL_RenderPresent(gRenderer);
			}

			while (Play_Again)
			{
				srand(time(NULL));
				int time = 0;
				int score = 0;
				int acceleration = 0;
				int frame_Character = 0;
				int frame_Enemy = 0;
				string highscore = GetHighScoreFromFile("high_score.txt");

				SDL_Event e;
				Enemy enemy1(ON_GROUND_ENEMY);
				Enemy enemy2(ON_GROUND_ENEMY);
				Enemy enemy3(IN_AIR_ENEMY);

				Mix_PlayMusic(gMusic, -1);
				GenerateEnemy(enemy1, enemy2, enemy3, gEnemyClips, gRenderer);

				int OffsetSpeed_Ground = BASE_OFFSET_SPEED;

				bool Quit = false;
				bool Game_State = true;
				while (!Quit)
				{
					if (Game_State)
					{
						UpdateGameTimeAndScore(time, acceleration, score);

						while (SDL_PollEvent(&e) != 0)
						{
							if (e.type == SDL_QUIT)
							{
								Quit = true;
								Play_Again = false;
							}

							HandlePauseButton(&e, gRenderer, gContinueButton,
								PauseButton, ContinueButton,
								gContinueButtonTexture, Game_State, gClick);

							character.HandleEvent(e, gJump);
						}
						SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
						SDL_RenderClear(gRenderer);

						SDL_RenderCopy(gRenderer, gBackgroundTexture.GetTexture(), NULL, NULL);
						RenderScrollingGround(OffsetSpeed_Ground, acceleration, gGroundTexture, gRenderer);


						character.Move();
						SDL_Rect* currentClip_Character = nullptr;
						if (character.OnGround())
						{
							currentClip_Character = &gCharacterClips[frame_Character / SLOW_FRAME_CHAR];
							character.Render(currentClip_Character, gRenderer, gCharacterTexture);
						}
						else
						{
							currentClip_Character = &gCharacterClips[0];
							character.Render(currentClip_Character, gRenderer, gCharacterTexture);
						}


						enemy1.Move(acceleration);
						enemy1.Render(gRenderer);

						enemy2.Move(acceleration);
						enemy2.Render(gRenderer);

						SDL_Rect* currentClip_Enemy = &gEnemyClips[frame_Enemy / SLOW_FRAME_ENEMY];
						enemy3.Move(acceleration);
						enemy3.Render(gRenderer, currentClip_Enemy);


						SDL_Rect* currentClip_Pause = &gPauseButton[PauseButton.currentSprite];
						PauseButton.Render(currentClip_Pause, gRenderer, gPauseButtonTexture);


						DrawPlayerScore(gText1Texture, gScoreTexture, textColor, gRenderer, gFont, score);
						DrawPlayerHighScore(gText2Texture, gHighScoreTexture, textColor, gRenderer, gFont, highscore);

						if (CheckEnemyColission(character,
							enemy1, enemy2, enemy3,
							currentClip_Character, currentClip_Enemy))
						{
							Mix_PauseMusic();
							Mix_PlayChannel(-1, gLose, 0);
                            UpdateHighScore("high_score.txt", score, highscore);
							Quit = true;
						}


						SDL_RenderPresent(gRenderer);

						ControlCharFrame(frame_Character);
						ControlEnemyFrame(frame_Enemy);
					}
				}

				DrawEndGameSelection(gLoseTexture, &e, gRenderer, Play_Again);
				if (!Play_Again)
				{
					enemy1.~Enemy();
					enemy2.~Enemy();
					enemy3.~Enemy();
				}
			}
		}
	}
	Close();

	return 0;
}


bool Init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_VIDEO) < 0)
	{
		cout<<"Can not initialize SDL."<<SDL_GetError;
		success = false;
	}
	else
	{
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
			cout << "Warning: Linear texture filtering not enabled!";

		gWindow = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
								   SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			cout<<"Can not create window"<<SDL_GetError;
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				cout<<"Can not create renderer"<<SDL_GetError;
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					cout<<"Can not initialize SDL_image"<<IMG_GetError;
					success = false;
				}

				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					cout<<"SDL_mixer could not initialize! SDL_mixer Error:"<< Mix_GetError();
					success = false;
				}

				if (TTF_Init() == -1)
				{
					cout<<"SDL_ttf could not initialize! SDL_ttf Error:"<<TTF_GetError();
					success = false;
				}
			}
		}
	}

	return success;
}

bool LoadMedia()
{
	bool success = true;

	gMusic = Mix_LoadMUS("sound/bkgr_audio.wav");
	if (gMusic == nullptr)
	{
		cout<<"Failed to load background music"<<Mix_GetError;
		success = false;
	}

	gMenuMusic = Mix_LoadMUS("sound/menu_audio.wav");
	if (gMenuMusic == nullptr)
	{
		cout<<"Failed to load menu music"<<Mix_GetError;
		success = false;
	}

	gClick = Mix_LoadWAV("sound/mouse_click.wav");
	if (gClick == nullptr)
	{
	    cout<<"Failed to load mouse click sound"<<Mix_GetError;
		success = false;
	}

	gJump = Mix_LoadWAV("sound/jump_sound.wav");
	if (gJump == nullptr)
	{
	    cout<<"Failed to load jumping sound"<<Mix_GetError;
		success = false;
	}

	gLose = Mix_LoadWAV("sound/lose_sound.wav");
	if (gLose == nullptr)
	{
	    cout<<"Failed to load lose sound"<<Mix_GetError;
		success = false;
	}

	else
	{
		gFont = TTF_OpenFont("font/pixel_font.ttf", 28);
		if (gFont == NULL)
		{
		    cout<<"Failed to load font"<<Mix_GetError;
			success = false;
		}
		else
		{
			if (!gText1Texture.LoadFromRenderedText("Your score: ", gFont, textColor, gRenderer))
			{
				cout << "Failed to render text1 texture" << endl;
				success = false;
			}

			if (!gText2Texture.LoadFromRenderedText("High score: ", gFont, textColor, gRenderer))
			{
				cout << "Failed to render text2 texture" << endl;
				success = false;
			}

			if (!gMenuTexture.LoadFromFile("imgs/background/menu.png", gRenderer))
			{
				cout << "Failed to load menu image" << endl;
				success = false;
			}

			if (!gInstructionTexture.LoadFromFile("imgs/background/instruction.png", gRenderer))
			{
				cout << "Failed to load instruction image" << endl;
				success = false;
			}

			if (!gPauseButtonTexture.LoadFromFile("imgs/button/big_button/pause_button.png", gRenderer))
			{
				cout << "Failed to load pause_button image " << endl;
				success = false;
			}
			else
			{
				for (int i = 0; i < 2; ++i)
				{
					gPauseButton[i].x = 22 * i;
					gPauseButton[i].y = 0;
					gPauseButton[i].w = 22;
					gPauseButton[i].h = 34;
				}
			}

			if (!gContinueButtonTexture.LoadFromFile("imgs/button/big_button/continue_button.png", gRenderer))
			{
				cout << "Failed to load continue_button image " << endl;
				success = false;
			}
			else
			{
				for (int i = 0; i < 2; ++i)
				{
					gContinueButton[i].x = 22 * i;
					gContinueButton[i].y = 0;
					gContinueButton[i].w = 22;
					gContinueButton[i].h = 34;
				}
			}

            if (!gBackgroundTexture.LoadFromFile("imgs/background/layer01.png", gRenderer))
            {
                cout << "Failed to load background image" << endl;
                success = false;
            }

			if (!gGroundTexture.LoadFromFile("imgs/background/ground.png", gRenderer))
			{
				cout << "Failed to load ground image" << endl;
				success = false;
			}

			if (!gCharacterTexture.LoadFromFile("imgs/character/character.png", gRenderer))
			{
				cout << "Failed to load character_run image." << endl;
				success = false;
			}
			else
			{
				gCharacterClips[0].x = 57 * 0;
				gCharacterClips[0].y = 0;
				gCharacterClips[0].w = 57;
				gCharacterClips[0].h = 57;

				gCharacterClips[1].x = 57 * 1;
				gCharacterClips[1].y = 0;
				gCharacterClips[1].w = 57;
				gCharacterClips[1].h = 57;

				gCharacterClips[2].x = 57 * 2;
				gCharacterClips[2].y = 0;
				gCharacterClips[2].w = 57;
				gCharacterClips[2].h = 57;

				gCharacterClips[3].x = 57 * 3;
				gCharacterClips[3].y = 0;
				gCharacterClips[3].w = 57;
				gCharacterClips[3].h = 57;

				gCharacterClips[4].x = 57 * 4;
				gCharacterClips[4].y = 0;
				gCharacterClips[4].w = 57;
				gCharacterClips[4].h = 57;

				gCharacterClips[5].x = 57 * 5;
				gCharacterClips[5].y = 0;
				gCharacterClips[5].w = 57;
				gCharacterClips[5].h = 57;
			}

			if (!gLoseTexture.LoadFromFile("imgs/background/lose.png", gRenderer))
			{
				cout << "Failed to load lose image." << endl;
				success = false;
			}
		}
	}
	return success;
}

void Close()
{
	gMenuTexture.Free();
	gInstructionTexture.Free();
	gCharacterTexture.Free();
	gGroundTexture.Free();
	gPlayButtonTexture.Free();
	gHelpButtonTexture.Free();
	gExitButtonTexture.Free();
	gBackButtonTexture.Free();
	gPauseButtonTexture.Free();
	gContinueButtonTexture.Free();
	gLoseTexture.Free();
	gText1Texture.Free();
	gScoreTexture.Free();
	gText2Texture.Free();
	gHighScoreTexture.Free();
    gBackgroundTexture.Free();

	Mix_FreeMusic(gMusic);
	Mix_FreeMusic(gMenuMusic);
	Mix_FreeChunk(gClick);
	Mix_FreeChunk(gLose);
	Mix_FreeChunk(gJump);
	gMusic = nullptr;
	gMenuMusic = nullptr;
	gClick = nullptr;
	gLose = nullptr;
	gJump = nullptr;

	SDL_DestroyRenderer(gRenderer);
	gRenderer = nullptr;

	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;

	IMG_Quit();
	Mix_Quit();
	SDL_Quit();
}
