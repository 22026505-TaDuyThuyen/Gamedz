
class TextObject
{
public:
	enum
	{
		RED_TEXT = 0,
		WHITE_TEXT = 1,
		BLACK_TEXT = 2,
		PINK_TEXT = 3,
	};
	SDL_Color GetColor() { return text_color; }
	void SetColor(const int &type);
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
	bool loadFromFile(std::string path);
	void free();
	void confirmOx_Oy(int x, int y)
	{
		Ox = x;
		Oy = y;
	}
	int getOx() { return Ox; }
	int getOy() { return Oy; }
	SDL_Rect getRect() { return hcn; }
	void SetRect()
	{
		hcn.x = Ox;
		hcn.y = Oy;
		hcn.w = mWidth;
		hcn.h = mHeight;
	}
	bool render(int x, int y, SDL_Rect *clip = NULL, double angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

private:
	std::string str;
	SDL_Color text_color;
	SDL_Texture *mTexture;
	int mWidth;
	int mHeight;
	int Ox, Oy;
	SDL_Rect hcn;
};

void TextObject::free()
{
	// Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}
bool TextObject::render(int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip)
{
	// Set rendering space and render to screen
	SDL_Rect renderQuad = {x, y, mWidth, mHeight};

	// Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	// Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}
bool TextObject::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	SDL_Surface *textSurface = TTF_RenderText_Solid(gFont1, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		// Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			// Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		// Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	// Return success
	return mTexture != NULL;
}

void TextObject::SetColor(const int &type)
{
	if (type == RED_TEXT)
	{
		text_color = {255, 0, 0};
	}
	else if (type == WHITE_TEXT)
	{
		text_color = {255, 255, 255};
	}
	else if (type == BLACK_TEXT)
	{
		text_color = {0, 0, 0};
	}
	else if (type == PINK_TEXT)
	{
		text_color = {255, 192, 203};
	}
}

bool TextObject::loadFromFile(std::string path)
{
	// Get rid of preexisting texture
	free();

	// The final texture
	SDL_Texture *newTexture = NULL;

	// Load image at specified path
	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		// Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGBA(loadedSurface->format, 0xFF, 0xFF, 0xFF, 0xFF));

		// Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			// Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		// Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	// Return success
	mTexture = newTexture;
	return mTexture != NULL;
}