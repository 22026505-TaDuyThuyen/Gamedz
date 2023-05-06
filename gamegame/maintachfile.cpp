#include "CommonFunc.h"
#include "BaseObject.h"
#include "BossObject.h"
#include "BulletObject.h"
#include "game_map.h"
#include "ExplosionObject.h"
#include "Geometric.h"
#include "ImpTimer.h"
#include "MainObject.h"
#include "PlayerPower.h"
#include "TextObject.h"
#include "ThreatsObject.h"

BaseObject g_background;
BaseObject g_menu ;
BaseObject g_game_over;
TTF_Font *font_time = NULL;
TextObject mark_game;
UINT mark_value = 0;

std ::vector<ThreatsObject *> MakeThreatList()
{
    std ::vector<ThreatsObject *> list_threats;
    ThreatsObject *dino_threats = new ThreatsObject[20];
    for (int i = 0; i < 20; i++)
    {
        ThreatsObject *p_threat = (dino_threats + i);
        if (p_threat != NULL)
        {
            p_threat->LoadImg("image//diano_left.png", g_screen);
            p_threat->set_clips();
            p_threat->set_type_move(ThreatsObject ::DINOSAUR_THREAT);
            p_threat->set_x_pos(25200+12*i);
            p_threat->set_y_pos(511);
            int pos1 = p_threat->get_x_pos() -700 -15*i ;
            int pos2 = 25120;
            p_threat->SetAnimationPos(pos1, pos2);
            p_threat->set_input_left(1);
            list_threats.push_back(p_threat);
        }
    }

    ThreatsObject *threats_objs = new ThreatsObject[20];

    for (int i = 0; i < 20; i++)
    {
        ThreatsObject *p_threat = (threats_objs + i);
        if (p_threat != NULL)
        {

            p_threat->LoadImg("image//threat_level.png", g_screen);
            p_threat->set_clips();
            p_threat->set_x_pos(700 + 1000 * i);
            p_threat->set_y_pos(0);
            p_threat->set_type_move(ThreatsObject ::STATIC_THREAT);
            p_threat->set_input_left(0);

            BulletObject *p_bullet = new BulletObject();
            p_threat->InitBullet(p_bullet, g_screen);
            list_threats.push_back(p_threat);
        }
    }

    ThreatsObject *dynamic_threats = new ThreatsObject[20];
    for (int i = 0; i < 20; i++)
    {
        ThreatsObject *p_threat = (dynamic_threats + i);
        if (p_threat != NULL)
        {
            p_threat->LoadImg("image//threat_left.png", g_screen);
            p_threat->set_clips();
            p_threat->set_type_move(ThreatsObject ::MOVE_IN_SPACE_THREAT);
            p_threat->set_x_pos(500 + i * 500);
            p_threat->set_y_pos(0);
            int pos1 = p_threat->get_x_pos() - 60;
            int pos2 = p_threat->get_x_pos() + 60;
            p_threat->SetAnimationPos(pos1, pos2);
            p_threat->set_input_left(1);
            list_threats.push_back(p_threat);
        }
    }
    return list_threats;
}

bool InitData()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0)
        return false;
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    g_window = SDL_CreateWindow("anh Thuyên súp bờ star", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (g_window == NULL)
    {
        success = false;
    }
    else
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen == NULL)
        {
            success = false;
        }
        else
        {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags))
            {
                success = false;
            }
        }

        if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
        {
            success = false;
        }

        g_sound_bullet[0] = Mix_LoadWAV(g_name_audio_bullet_main1);
        g_sound_bullet[1] = Mix_LoadWAV(g_name_audio_bullet_main2);
        g_sound_explosion = Mix_LoadWAV(g_name_audio_ex_main);
        g_sound_ex_main = Mix_LoadWAV(g_name_audio_ex_threats);
        g_click = Mix_LoadWAV(click);
        g_revive = Mix_LoadWAV(revive);
        g_dragon = Mix_LoadWAV(dragon);
        g_evil = Mix_LoadWAV(evil);
        g_win = Mix_LoadWAV(win);

        if (g_sound_bullet[0] == NULL || g_sound_bullet[1] == NULL || g_sound_explosion == NULL||g_click == NULL || g_revive == NULL||g_dragon ==NULL||g_evil ==NULL||g_win ==NULL)
        {
            return false;
        }

        if (TTF_Init() == -1)
        {
            success = false;
        }

        font_time = TTF_OpenFont("font//dlxfont.ttf", 15);
        if (font_time == NULL)
        {
            success = false;
        }
    }
    return success;
}
bool  LoadBackground()
{
    bool ret = g_background.LoadImg("image//back.png", g_screen);
    if (ret == false)
    {
        return false;
    }

    return true;
}

void close()
{
    g_background.free();
    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;
    SDL_DestroyWindow(g_window);
    g_window = NULL;
    IMG_Quit();
    SDL_Quit();
}

bool checkpositionforrect(const int& x, const int& y, SDL_Rect rect)
{
    if (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h)
    {
        return true;
    }
        return false;
}

int showMenu(SDL_Event g_event)
{
    g_menu.LoadImg("image//menu.png",g_screen);
    g_menu.Render(g_screen);

    const int kMenuItemNum =2 ;
    SDL_Rect pos_arr[kMenuItemNum];
    pos_arr[0].x = 200;
    pos_arr[0].y = 440;
    pos_arr[1].x = 1000;
    pos_arr[1].y = 440;

    TextObject  text_menu[kMenuItemNum];
    text_menu[0].SetText("PLAY GAME");
    text_menu[0].SetColor(TextObject :: BLACK_TEXT);
    text_menu[0].LoadFromRenderText(font_time,g_screen);
    text_menu[0].RenderText(g_screen, pos_arr[0].x , pos_arr[0].y);
    text_menu[0].SetRech(pos_arr[0].x , pos_arr[0].y);
    

    text_menu[1].SetText("EXIT");
    text_menu[1].SetColor(TextObject :: BLACK_TEXT);
    text_menu[1].LoadFromRenderText(font_time , g_screen);
    text_menu[1].RenderText(g_screen , pos_arr[1].x , pos_arr[1].y);
    text_menu[1].SetRech(pos_arr[1].x , pos_arr[1].y);
    
    bool selected [kMenuItemNum] = {0,0};

    int xm = 0;
    int ym = 0;

    while (true)
    {
        while(SDL_PollEvent(&g_event))
        {
            switch(g_event.type)
            {
                case SDL_QUIT :
                    return 1;
                case SDL_MOUSEMOTION:
                {
                    xm = g_event.motion.x;
                    ym = g_event.motion.y;
                    
                    for (int i =0; i<kMenuItemNum ; i++)
                    {
                        if(checkpositionforrect(xm , ym , text_menu[i].GetRech()))
                        {
                            if(selected[i] == false)
                            {
                                selected[i]= 1;
                                text_menu[i].SetColor(TextObject::RED_TEXT);
                                text_menu[i].LoadFromRenderText(font_time , g_screen);
                                text_menu[i].RenderText(g_screen, pos_arr[i].x , pos_arr[i].y);
                            }
                        }
                        else 
                        {
                            if(selected[i]==true)
                            {
                                selected[i]=0;
                                text_menu[i].SetColor(TextObject::BLACK_TEXT);
                                text_menu[i].LoadFromRenderText(font_time , g_screen);
                                text_menu[i].RenderText(g_screen, pos_arr[i].x , pos_arr[i].y);
                            }
                        }
                    }
                }
                break;
                case SDL_MOUSEBUTTONDOWN:
                {
                    xm = g_event.button.x;
                    ym = g_event.button.y;
                    for (int i =0; i<kMenuItemNum ; i++)
                    {
                        if(checkpositionforrect(xm , ym , text_menu[i].GetRech()))
                        {
                            Mix_PlayChannel(-1,g_click,0);
                            return i;
                        }
                    }
                }
                break; 
                case SDL_KEYDOWN:
                if(g_event.key.keysym.sym == SDLK_ESCAPE)
                {
                    return 1;
                }
                default: 
                break;
            }

        }
        SDL_RenderPresent(g_screen);
    }  
    return 1;
}

int ShowGameOver(SDL_Event g_event,bool is_win)
{
    const int Item =2 ;
    SDL_Rect pos_arr[Item];

    pos_arr[0].x = SCREEN_WIDTH*0.41;
    pos_arr[0].y = SCREEN_HEIGHT*0.85;
    pos_arr[1].x = SCREEN_WIDTH*0.41;
    pos_arr[1].y = SCREEN_HEIGHT*0.9;

    TextObject text_menu[Item];

    if(is_win ==false)
    {
        g_game_over.LoadImg("image//gameover.png",g_screen);
        g_game_over.Render(g_screen);
    }
    else if(is_win == true)
    {
        g_game_over.LoadImg("image//win.png",g_screen);
        g_game_over.Render(g_screen);
        Mix_PlayChannel(-1, g_win, 0);
    }
    
    text_menu[0].SetText("YOUR POINTS: ");
    text_menu[0].SetColor(TextObject :: YELLOW_TEXT);
    text_menu[0].LoadFromRenderText(font_time,g_screen);
    text_menu[0].RenderText(g_screen, pos_arr[0].x , pos_arr[0].y);
    text_menu[0].SetRech(pos_arr[0].x , pos_arr[0].y);

    text_menu[1].SetText("EXIT");
    text_menu[1].SetColor(TextObject :: YELLOW_TEXT);
    text_menu[1].LoadFromRenderText(font_time , g_screen);
    text_menu[1].RenderText(g_screen , pos_arr[1].x , pos_arr[1].y);
    text_menu[1].SetRech(pos_arr[1].x , pos_arr[1].y);

    bool selected = {0};

    int xm = 0;
    int ym = 0;

    while (true)
    {
        while(SDL_PollEvent(&g_event))
        {
            switch(g_event.type)
            {
                case SDL_QUIT :
                    return 1;
                case SDL_MOUSEMOTION:
                {
                    xm = g_event.motion.x;
                    ym = g_event.motion.y;

                    if(checkpositionforrect(xm , ym , text_menu[1].GetRech()))
                    {
                        if(selected== false)
                        {
                            selected= 1;
                            text_menu[1].SetColor(TextObject::RED_TEXT);
                            text_menu[1].LoadFromRenderText(font_time , g_screen);
                            text_menu[1].RenderText(g_screen, pos_arr[1].x , pos_arr[1].y);
                        }
                    }
                    else 
                    {
                        if(selected==true)
                        {
                            selected=0;
                            text_menu[1].SetColor(TextObject::YELLOW_TEXT);
                            text_menu[1].LoadFromRenderText(font_time , g_screen);
                            text_menu[1].RenderText(g_screen, pos_arr[1].x , pos_arr[1].y);
                        }
                    }
                    
                }
                break;
                case SDL_MOUSEBUTTONDOWN:
                {
                    xm = g_event.button.x;
                    ym = g_event.button.y;
                    
                    if(checkpositionforrect(xm , ym , text_menu[1].GetRech()))
                    {
                        return 1;
                    }
            
                }
                break; 
                case SDL_KEYDOWN:
                if(g_event.key.keysym.sym == SDLK_ESCAPE)
                {
                    return 1;
                }
                default: 
                break;
            }
        }
        std ::string mark_game_over = std :: to_string(mark_value);
        mark_game.SetText(mark_game_over);
        mark_game.SetColor(TextObject :: YELLOW_TEXT);
        mark_game.LoadFromRenderText(font_time, g_screen);
        mark_game.RenderText(g_screen, SCREEN_WIDTH*0.55, SCREEN_HEIGHT*0.85);

        SDL_RenderPresent(g_screen);
    }  
    return 1;
}

int main(int argc, char *argv[])
{
    ImpTimer fps_timer;  
    if (InitData() == false)
    {
        return -1;
    }
    if (LoadBackground() == false)
    {
        return -1;
    }

    GameMap game_map;
    game_map.LoadMap("map//map01.dat");
    game_map.LoadTiles(g_screen);

    MainObject p_player;
    p_player.LoadImg("image//player_right.png", g_screen);
    p_player.set_clips();

    PlayerPower player_power;
    player_power.Init(g_screen);

    PlayerMoney player_money;
    player_money.Init(g_screen);
    player_money.SetPos(SCREEN_WIDTH * 0.5 - 350, 8);

    std ::vector<ThreatsObject *> threats_list = MakeThreatList();

    BossObject bossObject;
    bossObject.LoadImg("image//boss_object.png", g_screen);
    bossObject.set_clips();
    int xPosBoss = MAX_MAP_X * TILE_SIZE - (SCREEN_WIDTH * 0.2);
    bossObject.set_xpos(xPosBoss);
    bossObject.set_ypos(200);
    bossObject.InitBullet(g_screen);

    ExplosionObject exp_threat;
    bool tRet = exp_threat.LoadImg("image//ex3.png", g_screen);
    if (tRet == false)
    {
        return -1;
    }
    else
    {
        exp_threat.set_clips();
    }

    ExplosionObject exp_main;
    bool mRet = exp_main.LoadImg("image//ex1.png", g_screen);
    if (mRet == false)
    {
        return -1;
    }
    else
    {
        exp_main.set_clips();
    }

    ExplosionObject ex_boss;
    bool bRet = ex_boss.LoadImg("image//ex2.png", g_screen);
    if (bRet == false)
    {
        return -1;
    }
    else
    {
        ex_boss.set_clips();
    }

    int num_die = 0;
    int num_boss_die = 0 ; 
    
    TextObject time_game;
    time_game.SetColor(TextObject ::WHITE_TEXT);
    mark_game.SetColor(TextObject ::BLACK_TEXT);
    
    TextObject money_game;
    money_game.SetColor(TextObject ::YELLOW_TEXT);
    
    TextObject boss_game;
    boss_game.SetColor(TextObject :: RED_TEXT);
    UINT boss_health = 100;

    bool quit = false;
    int check_menu = showMenu(g_event);
    if(check_menu == 1) quit = true;
    int time = 300;
    int frame1s = 0;
    while (!quit)
    {
        fps_timer.start();
        while (SDL_PollEvent(&g_event) != 0)
        {
            if (g_event.type == SDL_QUIT)
            {
                quit = true;
            }
            p_player.HandelInputAction(g_event, g_screen);
        }

        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(g_screen);

        frame1s ++;
        if(frame1s == FRAME_PER_SECOND)
        {
            time --;
            frame1s =0 ;
        }
        g_background.Render(g_screen, NULL);
        Map map_data = game_map.getMap();

        p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
        p_player.DoPlayer(map_data);
        p_player.Show(g_screen);

        game_map.SetMap(map_data);
        game_map.DrawMap(g_screen);
        p_player.HandleBullet(g_screen);

        GeometricFormat rectangle_size(0, 0, SCREEN_WIDTH, 40);
        ColorData color_data(0, 80, 150);
        Geometric ::RenderRectangle(rectangle_size, color_data, g_screen);

        GeometricFormat outLineSize(1, 1, SCREEN_WIDTH - 1, 38);
        ColorData color_data2(255, 255, 255);
        Geometric ::RenderOutline(outLineSize, color_data2, g_screen);

        player_power.Show(g_screen);
        player_money.Show(g_screen);
        int money_count = p_player.GetMoneyCount();
        for (int i =0;i<threats_list.size();i++)
        {
            ThreatsObject* p_threat = threats_list.at(i);
            if(i<20)
            {
                if(p_threat != NULL)
                {
                    int val = MAX_MAP_X*TILE_SIZE - (map_data.start_x_ + p_player.GetRect().x) ;
                    if(val <= 2*SCREEN_WIDTH)
                    {
                        p_threat -> SetMapXY(map_data.start_x_ , map_data.start_y_);
                        p_threat -> ImpMoveType(g_screen);
                        p_threat -> DoPlayer(map_data);
                        p_threat -> Show(g_screen);
                    }
                }
            }

            if(i>=20)
            {
                if(p_threat != NULL)
                {
                    p_threat -> SetMapXY(map_data.start_x_ , map_data.start_y_);
                    p_threat -> ImpMoveType(g_screen);
                    p_threat -> DoPlayer(map_data);
                    p_threat -> MakeBullet(g_screen,SCREEN_WIDTH,SCREEN_HEIGHT);
                    p_threat -> Show(g_screen);
                }
            }

            SDL_Rect rect_player = p_player.GetRectFrame();
                bool bCol1 = false;
                std::vector<BulletObject*> tBullet_list = p_threat->get_bullet_list();
                for (int jj = 0; jj < tBullet_list.size(); ++jj)
                {
                    BulletObject* pt_bullet = tBullet_list.at(jj);

                    if (pt_bullet)
                    {
                        bCol1 = SDLCommonFunc::CheckCollision(pt_bullet->GetRect(), rect_player);
                        if (bCol1)
                        {
                            p_threat->RemoveBullet(jj);
                            break;
                        }
                    }
                }
            SDL_Rect rect_threat = p_threat ->GetRectFrame();
            bool bCol2 = SDLCommonFunc :: CheckCollision(rect_player, rect_threat);

            if(bCol1 ||bCol2  )
            {
                int width_exp_frame = exp_main.get_frame_width();
                int heiht_exp_height = exp_main.get_frame_height();
                for (int ex = 0; ex <NUM_FRAME_EXP ; ex++)
                {
                    int x_pos = (p_player.GetRect().x + p_player.get_frame_width()*0.5) - width_exp_frame*0.5;
                    int y_pos = (p_player.GetRect().y + p_player.get_frame_height()*0.5) - heiht_exp_height*0.5;

                    exp_main.set_frame(ex);
                    exp_main.SetRect(x_pos, y_pos);
                    exp_main.Show(g_screen);
                    SDL_RenderPresent(g_screen);
                }

                Mix_PlayChannel(-1, g_sound_ex_main, 0);
                num_die ++;
                if(num_die <=3)
                {
                    p_player.SetRect(0,0);
                    p_player.set_comeback_time(60);
                    SDL_Delay(1000);
                    player_power.Decrease();
                    player_power.Render(g_screen);
                    continue;
                }
                else 
                {
                    SDL_Delay(1000);
                    if(num_die ==4 && money_count >=MONEY_REVIVE)
                    {
                        p_player.setpos(0,200);
                        p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
                        p_player.DoPlayer(map_data);
                        quit =false;      
                        
                     }
                     else if(num_die ==4 && money_count <MONEY_REVIVE || num_die ==5)
                     {
                        Mix_PlayChannel(-1,g_evil,0);
                        SDL_Delay(2000);
                        int check =ShowGameOver(g_event,false);
                        if(check ==1)
                        {
                            p_threat -> free();
                            close();
                            quit = true;
                            return 0;
                        }
                     }
                }
            }
        }
        int frame_exp_width = exp_threat.get_frame_width();
        int frame_exp_height = exp_threat.get_frame_height();

        std ::vector<BulletObject *> bullet_arr = p_player.get_bullet_list();
        for (int k = 0; k < bullet_arr.size(); k++)
        {
            BulletObject *p_bullet = bullet_arr.at(k);
            if (p_bullet != NULL)
            {
                for (int t = 0; t < threats_list.size(); t++)
                {
                    ThreatsObject *obj_threat = threats_list.at(t);
                    if (obj_threat != NULL)
                    {
                        SDL_Rect tRect;
                        tRect.x = obj_threat->GetRect().x;
                        tRect.y = obj_threat->GetRect().y;
                        tRect.w = obj_threat->get_width_frame();
                        tRect.h = obj_threat->get_height_frame();

                        SDL_Rect bRect = p_bullet->GetRect();
                        bool bCollision = SDLCommonFunc ::CheckCollision(bRect, tRect);
                        if (bCollision == true)
                        {
                            mark_value++;
                            for (int ex = 0; ex < NUM_FRAME_EXP; ex++)
                            {
                                int x_pos = p_bullet->GetRect().x - frame_exp_width * 0.5;
                                int y_pos = p_bullet->GetRect().y - frame_exp_height * 0.5;

                                exp_threat.set_frame(ex);
                                exp_threat.SetRect(x_pos, y_pos);
                                exp_threat.Show(g_screen);
                            }

                            p_player.RemoveBullet(k);
                            obj_threat->free();
                            threats_list.erase(threats_list.begin() + t);

                            Mix_PlayChannel(-1, g_sound_explosion, 0);
                        }
                    }
                }
            }
        }
        if (!bossObject.getDie())
        {
            int frame_exp_boss_width = ex_boss.get_frame_width();
            int frame_exp_boss_height = ex_boss.get_frame_height();
            std::vector<BulletObject *> bullet_for_boss = p_player.get_bullet_list();
            for (int i = 0; i < bullet_for_boss.size(); i++)
            {
                BulletObject *p_bullet = bullet_for_boss.at(i);
                if (p_bullet != NULL)

                {
                    SDL_Rect tRect;
                    tRect.x = bossObject.GetRect().x;
                    tRect.y = bossObject.GetRect().y;
                    tRect.w = bossObject.get_width_frame();
                    tRect.h = bossObject.get_height_frame();

                    SDL_Rect bRect = p_bullet->GetRect();
                    bool bCol3 = false;
                    bCol3 = SDLCommonFunc ::CheckCollision(bRect, tRect);

                    if (bCol3 == true)
                    {
                        mark_value+=2;
                        for (int ex = 0; ex < NUM_FRAME_EXP; ex++)
                        {
                            int x_pos = p_bullet->GetRect().x - frame_exp_boss_width * 0.5;
                            int y_pos = p_bullet->GetRect().y - frame_exp_boss_height * 0.5;

                            ex_boss.set_frame(ex);
                            ex_boss.SetRect(x_pos, y_pos);
                            ex_boss.Show(g_screen);
                            Mix_PlayChannel(-1, g_sound_ex_main, 0);
                            
                        }
                        p_player.RemoveBullet(i);
                        num_boss_die ++;
                        boss_health --;
                        if(num_boss_die >= 100)
                        {
                            p_player.RemoveBullet(i);
                            bossObject.free();
                            bossObject.setDie();
                            SDL_Delay(2000);
                            int check_ = ShowGameOver(g_event,true);
                            if(check_ ==1)
                            {
                                p_player.free();
                                quit = true;
                            }
                        }
                    }
                }
            }
        }

        if (!bossObject.getDie())
        {
            std::vector<BulletObject *> bullet_of_boss = bossObject.get_bullet_list();
            SDL_Rect rect_boss = bossObject.GetRectFrame();
            SDL_Rect rect_player = p_player.GetRectFrame();

            bool bCol5 = false;
            for (int a = 0; a < bullet_of_boss.size(); a++)
            {
                BulletObject *boss_bullet = bullet_of_boss.at(a);
                if (boss_bullet)
                {
                    bCol5 = SDLCommonFunc ::CheckCollision(boss_bullet->GetRect(), rect_player);
                    if (bCol5)
                    {
                        bossObject.RemoveBullet(a);
                        break;
                    }
                }
            }

            bool bCol6 = false;
            bCol6 = SDLCommonFunc::CheckCollision(rect_boss, rect_player);
            if (bCol5 == true || bCol6 == true)
            {               

                int width_exp_frame = exp_main.get_frame_width();
                int heiht_exp_height = exp_main.get_frame_height();
                for (int ex = 0; ex <NUM_FRAME_EXP ; ex++)
                {
                    int x_pos = (p_player.GetRect().x + p_player.get_frame_width()*0.5) - width_exp_frame*0.5;
                    int y_pos = (p_player.GetRect().y + p_player.get_frame_height()*0.5) - heiht_exp_height*0.5;

                    exp_main.set_frame(ex);
                    exp_main.SetRect(x_pos, y_pos);
                    exp_main.Show(g_screen);
                    SDL_RenderPresent(g_screen);
                }
                Mix_PlayChannel(-1, g_sound_ex_main, 0);
                num_die ++;
                if(num_die <=3)
                {
                    p_player.SetRect(0,0);
                    p_player.set_comeback_time(60);
                    SDL_Delay(1000);
                    player_power.Decrease();
                    player_power.Render(g_screen);
                    continue;
                }
                else 
                {
                    SDL_Delay(1000);
                    if(num_die ==4 && money_count >=15)
                    {
                        p_player.setpos(MAX_MAP_X*TILE_SIZE - SCREEN_WIDTH,200);
                        p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
                        p_player.DoPlayer(map_data);
                        quit =false;      
                     }
                     else if(num_die ==4 && money_count <15 || num_die ==5)
                     {
                        Mix_PlayChannel(-1, g_dragon,0);
                        SDL_Delay(2000);
                        int check =ShowGameOver(g_event,false);
                        if(check ==1)
                        {
                            bossObject.free();
                            close();
                            quit = true;
                            return 0;
                        }
                     }
                }      
            }
        }

        std::string str_time = "TIME: ";
        Uint32 val_time = time;
        if (val_time <= 0)
        {
            if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "h", "d", g_window) == 0)
            {
                quit = true;
                break;
            }
        }
        else
        {
            std ::string str_val = std::to_string(val_time);
            str_time += str_val;
            //truyen noi dung vao
            time_game.SetText(str_time);
            time_game.LoadFromRenderText(font_time, g_screen);
            time_game.RenderText(g_screen, SCREEN_WIDTH - 150, 15);
        }

        std::string val_str_mark = std::to_string(mark_value);
        std::string strMark("MARK: ");
        strMark += val_str_mark;

        mark_game.SetText(strMark);
        mark_game.LoadFromRenderText(font_time, g_screen);
        mark_game.RenderText(g_screen, SCREEN_WIDTH * 0.5-100, 15);

        if(num_die<=3)
        {
        std::string money_str = std::to_string(money_count);
        money_game.SetText(money_str);
        money_game.LoadFromRenderText(font_time, g_screen);
        money_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 300, 15);
        }

        if(num_die ==4)
        {
            money_count -=MONEY_REVIVE;
            std::string money_str_new = std::to_string(money_count);
            money_game.SetText(money_str_new);
            money_game.LoadFromRenderText(font_time, g_screen);
            money_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 300, 15);
        }
        
        std :: string val_str_health = std :: to_string(boss_health);
        std :: string strBoss("BOSS'S HP: ");
        strBoss += val_str_health;
        boss_game.SetText(strBoss);
        boss_game.LoadFromRenderText(font_time, g_screen);
        boss_game.RenderText(g_screen,SCREEN_WIDTH-500,15);

        int val = MAX_MAP_X * TILE_SIZE - (map_data.start_x_ + p_player.GetRect().x);
        if (val <=  SCREEN_WIDTH)
        {
            if (!bossObject.getDie())
            {
                bossObject.SetMapXY(map_data.start_x_, map_data.start_y_);
                bossObject.DoPlayer(map_data);
                bossObject.MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);          
                bossObject.Show(g_screen);      
            }
        }

        SDL_RenderPresent(g_screen);
        // thời gian thực hiện trò chơi trong 1 khung hình
        int real_imp_time = fps_timer.get_ticks();
        // thời gian chạy cho 1 frame
        int time_one_frame = 1000 / FRAME_PER_SECOND;
        // nếu real time lớn hơn time one frame thì không cần độ trễ
        if (real_imp_time < time_one_frame)
        {
            // độ trễ
            int delay_time = time_one_frame - real_imp_time;
            if (delay_time >= 0)
            {
                SDL_Delay(delay_time);
            }
        }
    }
    for (int i = 0; i < threats_list.size(); i++)
    {
        ThreatsObject *p_threat = threats_list.at(i);
        if (p_threat)
        {
            p_threat->free();
            p_threat = NULL;
        }
    }
    threats_list.clear();
    bossObject.free();
    close();
    return 0;
}
