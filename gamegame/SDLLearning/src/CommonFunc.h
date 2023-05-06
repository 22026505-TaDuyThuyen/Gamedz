#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

#include <windows.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>

static SDL_Window *g_window = NULL;
static SDL_Renderer *g_screen = NULL;
static SDL_Event g_event;

static Mix_Music *g_music = NULL;
static Mix_Chunk *g_sound_bullet[3];
static Mix_Chunk *g_sound_explosion = NULL;
static Mix_Chunk *g_sound_ex_main = NULL;
static Mix_Chunk * g_click = NULL;
static Mix_Chunk* g_revive = NULL;
static Mix_Chunk* g_dragon = NULL;
static Mix_Chunk * g_evil = NULL;
static Mix_Chunk * g_win = NULL;

static char g_name_audio_bullet_main1[] = {"sound//Fire.wav"};
static char g_name_audio_bullet_main2[] = {"sound//Laser.wav"};
static char g_name_audio_ex_main[] = {"sound//Explosion+1.wav"};
static char g_name_audio_ex_threats[] = {"sound//Bomb1.wav"};
static char g_name_sound_increase[] = {"sound//two_beep_.wav"};
static char kSoundBeep[] = {"sound//beep_.wav"};
static char click[] = {"sound//click.wav"};
static char revive[] = {"sound//revive.wav"};
static char dragon[] = {"sound//dragon.wav"};
static char evil[] = {"sound//evil.wav"};
static char win[] = {"sound//winwin.wav"};

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