#include "CommonFunc.h"

 SDL_Window *g_window =NULL;
 SDL_Renderer *g_screen =NULL;
 SDL_Event g_event;

 Mix_Music *g_music = NULL;
 Mix_Chunk *g_sound_bullet[3];
 Mix_Chunk *g_sound_explosion = NULL;
 Mix_Chunk *g_sound_ex_main = NULL;
 Mix_Chunk * g_click = NULL;
 Mix_Chunk* g_revive = NULL;
 Mix_Chunk* g_dragon = NULL;
 Mix_Chunk * g_evil = NULL;
 Mix_Chunk * g_win = NULL;

 char g_name_audio_bullet_main1[] = {"sound//Fire.wav"};
 char g_name_audio_bullet_main2[] = {"sound//Laser.wav"};
 char g_name_audio_ex_main[] = {"sound//Explosion+1.wav"};
 char g_name_audio_ex_threats[] = {"sound//Bomb1.wav"};
 char g_name_sound_increase[] = {"sound//two_beep_.wav"};
 char kSoundBeep[] = {"sound//beep_.wav"};
 char click[] = {"sound//click.wav"};
 char revive[] = {"sound//revive.wav"};
 char dragon[] = {"sound//dragon.wav"};
 char evil[] = {"sound//evil.wav"};
 char win[] = {"sound//winwin.wav"};


bool SDLCommonFunc ::CheckCollision(const SDL_Rect &object1, const SDL_Rect &object2)
{
    int left_a = object1.x;
    int right_a = object1.x + object1.w;
    int top_a = object1.y;
    int bottom_a = object1.y + object1.h;

    int left_b = object2.x;
    int right_b = object2.x + object2.w;
    int top_b = object2.y;
    int bottom_b = object2.y + object2.h;

    // Case 1: size object 1 < size object 2
    if (left_a > left_b && left_a < right_b)
    {
        if (top_a > top_b && top_a < bottom_b)
        {
            return true;
        }
    }

    if (left_a > left_b && left_a < right_b)
    {
        if (bottom_a > top_b && bottom_a < bottom_b)
        {
            return true;
        }
    }

    if (right_a > left_b && right_a < right_b)
    {
        if (top_a > top_b && top_a < bottom_b)
        {
            return true;
        }
    }

    if (right_a > left_b && right_a < right_b)
    {
        if (bottom_a > top_b && bottom_a < bottom_b)
        {
            return true;
        }
    }

    // Case 2: size object 1 < size object 2
    if (left_b > left_a && left_b < right_a)
    {
        if (top_b > top_a && top_b < bottom_a)
        {
            return true;
        }
    }

    if (left_b > left_a && left_b < right_a)
    {
        if (bottom_b > top_a && bottom_b < bottom_a)
        {
            return true;
        }
    }

    if (right_b > left_a && right_b < right_a)
    {
        if (top_b > top_a && top_b < bottom_a)
        {
            return true;
        }
    }

    if (right_b > left_a && right_b < right_a)
    {
        if (bottom_b > top_a && bottom_b < bottom_a)
        {
            return true;
        }
    }

    // Case 3: size object 1 = size object 2
    if (top_a == top_b && right_a == right_b && bottom_a == bottom_b)
    {
        return true;
    }

    return false;
}