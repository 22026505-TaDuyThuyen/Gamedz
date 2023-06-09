#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

#include <windows.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>
#include <stdio.h>
#include <tchar.h>
#include <SDKDDKVer.h>

extern SDL_Window *g_window ;
extern SDL_Renderer *g_screen ;
extern SDL_Event g_event;

extern Mix_Music *g_music ;
extern Mix_Chunk *g_sound_bullet[3];
extern Mix_Chunk *g_sound_explosion ;
extern Mix_Chunk *g_sound_ex_main ;
extern Mix_Chunk * g_click ;
extern Mix_Chunk* g_revive ;
extern Mix_Chunk* g_dragon ;
extern Mix_Chunk * g_evil ;
extern Mix_Chunk * g_win ;

extern char g_name_audio_bullet_main1[] ;
extern char g_name_audio_bullet_main2[] ;
extern char g_name_audio_ex_main[]  ;
extern char g_name_audio_ex_threats[] ;
extern char g_name_sound_increase[] ;
extern char kSoundBeep[] ;
extern char click[] ;
extern char revive[] ;
extern char dragon[] ;
extern char evil[];
extern char win[] ;

const int FRAME_PER_SECOND = 25;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int RENDER_DRAW_COLOR = 0Xff;

#define BLANK_TILE 0
#define TILE_SIZE 64
#define MAX_MAP_X 400 
#define MAX_MAP_Y 10  
#define STATE_MONEY 4
#define MONEY_REVIVE 25

typedef struct Input
{
   
    int left_;
    int right_;
    int up_;
    int down_;
    int jump_;
};
typedef struct Map
{

    int start_x_;
    int start_y_;

    int max_x_;
    int max_y_;

    int tile[MAX_MAP_Y][MAX_MAP_X];
    char *file_name_;
};
namespace SDLCommonFunc
{
    bool CheckCollision(const SDL_Rect &object1, const SDL_Rect &object2);

}  

#endif