bool checkpositionforrect(const int &x, const int &y, SDL_Rect rect)
{
	if (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h)
	{
		return true;
	}
	return false;
}


int showMenu(SDL_Event e)
{
	SDL_Rect item[itemMenu];
	item[0].x = 150;
	item[0].y = 254;
	item[1].x = 160;
	item[1].y = 338;

	int x, y;
	TextObject text_menu[itemMenu];
	text_menu[0].SetColor(0);
	text_menu[0].loadFromRenderedText("Play Game", text_menu[0].GetColor());
	text_menu[0].confirmOx_Oy(item[0].x, item[0].y);
	text_menu[0].SetRect();

	text_menu[1].SetColor(0);
	text_menu[1].loadFromRenderedText("Exit", text_menu[1].GetColor());
	text_menu[1].confirmOx_Oy(item[1].x, item[1].y);
	text_menu[1].SetRect();

	bool Selected[itemMenu] = {0, 0};
	bool quit = false;
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			switch (e.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_MOUSEMOTION:
			{
				SDL_GetMouseState(&x, &y);
				for (int i = 0; i < itemMenu; i++)
				{
					if (checkpositionforrect(x, y, text_menu[i].getRect()))
					{
						if (Selected[i] == 0)
						{
							Selected[i] = 1;
							text_menu[i].SetColor(2);
						}
					}
					else
					{
						if (Selected[i] == 1)
						{
							Selected[i] = 0;
							text_menu[i].SetColor(0);
						}
					}
				}
			}
			break;
			case SDL_MOUSEBUTTONDOWN:
			{
				SDL_GetMouseState(&x, &y);
				for (int i = 0; i < itemMenu; i++)
				{
					if (checkpositionforrect(x, y, text_menu[i].getRect()))
					{
						return i;
					}
				}
			}
			break;
			case SDL_KEYDOWN:
			{
				if (e.key.keysym.sym == SDLK_ESCAPE)
				{
					return 1;
				}
			}
			default:
				break;
			}
		}
		SDL_RenderClear(gRenderer);

		bkMENU.render(0, 0);
		for (int i = 0; i < itemMenu; i++)
		{
			if (i == 0)
				text_menu[i].loadFromRenderedText("Play Game", text_menu[i].GetColor());
			if (i == 1)
				text_menu[i].loadFromRenderedText("Exit", text_menu[i].GetColor());
			text_menu[i].render(text_menu[i].getOx(), text_menu[i].getOy());
		}

		SDL_RenderPresent(gRenderer);
	}
	return 1;
}