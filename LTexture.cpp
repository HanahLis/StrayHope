#include "LTexture.h"

LTexture::LTexture()
{
	mTexture = nullptr;

	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	Free();
}

void LTexture::Free()
{
	if (mTexture != nullptr)
	{
		mTexture = nullptr;
		mWidth = 0;
		mHeight = 0;
	}
}

bool LTexture::LoadFromRenderedText(string textureText,TTF_Font *gFont, SDL_Color textColor, SDL_Renderer *gRenderer)
{
	Free();

	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		cout<<"Unable to render text surface! SDL_ttf Error:"<<TTF_GetError();
	}
	else
	{
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			cout<<"Unable to render text surface! SDL_ttf Error:"<<TTF_GetError();
		}
		else
		{
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}
		SDL_FreeSurface(textSurface);
	}

	return mTexture != NULL;
}

bool LTexture::LoadFromFile(string path, SDL_Renderer *gRenderer)
{
	Free();

	SDL_Texture* tmpTexture = nullptr;

	SDL_Surface* tmpSurface = IMG_Load(path.c_str());
	if (tmpSurface == nullptr)
	{
		cout<<"Can not load image."<<IMG_GetError;
	}
	else
	{
		SDL_SetColorKey(tmpSurface, SDL_TRUE, SDL_MapRGB(tmpSurface->format, 0, 255, 255));

		tmpTexture = SDL_CreateTextureFromSurface(gRenderer, tmpSurface);
		if (tmpTexture == nullptr)
		{
            cout<<"Can not create texture from surface."<<SDL_GetError;
		}
		else
		{
			mWidth = tmpSurface->w;
			mHeight = tmpSurface->h;
		}

		SDL_FreeSurface(tmpSurface);
	}

	mTexture = tmpTexture;

	return mTexture != nullptr;
}

void LTexture::Render(int x, int y, SDL_Renderer* gRenderer, SDL_Rect* clip)
{
	SDL_Rect renderSpace = { x, y, mWidth, mHeight };

	if (clip != nullptr)
	{
		renderSpace.w = clip->w;
		renderSpace.h = clip->h;
	}

	SDL_RenderCopy(gRenderer, mTexture, clip, &renderSpace);
}

int LTexture::GetWidth()
{
	return mWidth;
}

int LTexture::GetHeight()
{
	return mHeight;
}

