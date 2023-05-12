#include "MainObject.h"

MainObject::MainObject()
{
    frame_ = 0;
    x_pos_ = 23000;
    y_pos_ = 500;
    x_val_ = 0;
    y_val_ = 0;
    width_frame_ = 0;
    height_frame_ = 0;
    // chua biet status la ben trai hay ben phai
    status_ = WALK_NONE;
    
    input_type_.left_ = 0;
    input_type_.right_ = 0;
    input_type_.jump_ = 0;
    input_type_.down_ = 0;
    input_type_.up_ = 0;
    
    on_ground_ = false;
    
    map_x_ = 0;
    map_y_ = 0;
    come_back_time_ = 0;
    money_count = 0;
}
MainObject::~MainObject()
{
}

// định nghĩa ra hàm loadimg
bool MainObject::LoadImg(std::string path, SDL_Renderer *screen)
{
    bool ret = BaseObject::LoadImg(path, screen);

    if (ret == true)
    {
        width_frame_ = rect_.w / 8;
        height_frame_ = rect_.h;
    }
    return ret;
}

SDL_Rect MainObject ::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;

    return rect;
}

void MainObject::set_clips()
{
    if (width_frame_ > 0 && height_frame_ > 0)
    {
        frame_clip_[0].x = 0;
        frame_clip_[0].y = 0;
        frame_clip_[0].w = width_frame_;
        frame_clip_[0].h = height_frame_;

        frame_clip_[1].x = width_frame_;
        frame_clip_[1].y = 0;
        frame_clip_[1].w = width_frame_;
        frame_clip_[1].h = height_frame_;

        frame_clip_[2].x = 2 * width_frame_;
        frame_clip_[2].y = 0;
        frame_clip_[2].w = width_frame_;
        frame_clip_[2].h = height_frame_;

        frame_clip_[3].x = 3 * width_frame_;
        frame_clip_[3].y = 0;
        frame_clip_[3].w = width_frame_;
        frame_clip_[3].h = height_frame_;

        frame_clip_[4].x = 4 * width_frame_;
        frame_clip_[4].y = 0;
        frame_clip_[4].w = width_frame_;
        frame_clip_[4].h = height_frame_;

        frame_clip_[5].x = 5 * width_frame_;
        frame_clip_[5].y = 0;
        frame_clip_[5].w = width_frame_;
        frame_clip_[5].h = height_frame_;

        frame_clip_[6].x = 6 * width_frame_;
        frame_clip_[6].y = 0;
        frame_clip_[6].w = width_frame_;
        frame_clip_[6].h = height_frame_;

        frame_clip_[7].x = 7 * width_frame_;
        frame_clip_[7].y = 0;
        frame_clip_[7].w = width_frame_;
        frame_clip_[7].h = height_frame_;
    }
}
void MainObject::Show(SDL_Renderer *des)
{
    UpdateImagePlayer(des);

    if (input_type_.left_ == 1 || input_type_.right_ == 1)
    {
        frame_++;
    }
    else
    {
        frame_ = 0;
    }

    if (frame_ >= 8)
    {
        frame_ = 0;
    }
    // để bớt được thời gian xử lí khi nhân vật ở ngoài bản đồ
    if (come_back_time_ == 0)
    {
        // vi tri hien thi nhan vat xet sau
        // màn hình sẽ hiển thị từ map_x trở đi nên phải trừ đi map_x rect_.x và rect_.y là vị trí chính xác của nhân vật trên bản đồ
        rect_.x = x_pos_ - map_x_;
        rect_.y = y_pos_ - map_y_;
        // xem hien tai dang o clip nao
        SDL_Rect *current_clip = &frame_clip_[frame_];
        // renderquad là kích thước cuối cùng chứa chính xác vị trí ,kích thước, chiều dài chiều rộng là bao nhiêu
        SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame_, height_frame_};

        SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
    }
}

void MainObject::HandelInputAction(SDL_Event events, SDL_Renderer *screen)
{
    // su kien an phim
    if (events.type == SDL_KEYDOWN)
    {
        switch (events.key.keysym.sym)
        {
        case SDLK_RIGHT:
        {
            status_ = WALK_RIGHT;
            input_type_.right_ = 1;
            input_type_.left_ = 0; 

            UpdateImagePlayer(screen);
        }
        break;
        case SDLK_LEFT:
        {
            status_ = WALK_LEFT;
            input_type_.left_ = 1;
            input_type_.right_ = 0; 
            UpdateImagePlayer(screen);
        }

        break;
        }
    }
    // su kien tha phim
    else if (events.type == SDL_KEYUP)
    {
        switch (events.key.keysym.sym)
        {
        case SDLK_RIGHT:
        {

            input_type_.right_ = 0;
        }
        break;
        case SDLK_LEFT:
        {

            input_type_.left_ = 0;
        }
        break;
        }
    }
    if (events.type == SDL_MOUSEBUTTONDOWN)
    {
        if (events.button.button == SDL_BUTTON_RIGHT)
        {
            input_type_.jump_ = 1;
        }
        else if (events.button.button == SDL_BUTTON_LEFT)
        {
            // TẠO RA MỘT VIÊN ĐẠN MỚI
            BulletObject *p_bullet = new BulletObject();
            p_bullet->set_bullet_type(BulletObject ::SPHERE_BULLET);
            p_bullet->LoadImgBullet(screen);

            Mix_PlayChannel(-1, g_sound_bullet[0], 1);
            if (status_ == WALK_LEFT)
            {
                p_bullet->set_bullet_dir(BulletObject ::DIR_LEFT);

                p_bullet->SetRect(this->rect_.x, rect_.y + height_frame_ * 0.25);
            }
            else
            {
                p_bullet->set_bullet_dir(BulletObject ::DIR_RIGHT);

                p_bullet->SetRect(this->rect_.x + width_frame_ - 20, rect_.y + height_frame_ * 0.25);
            }

            // set vị trí cho viên đạn xuất phát từ đâu

            // set tốc độ viên đạn
            p_bullet->set_x_val(20);
            p_bullet->set_is_move(true);

            // nap vien dan vao bawng
            p_bullet_list_.push_back(p_bullet);
        }
    }
}

void MainObject ::HandleBullet(SDL_Renderer *des)
{
    for (int i = 0; i < p_bullet_list_.size(); i++)
    {
        // lấy đạn ra từ băng đạn
        BulletObject *p_bullet = p_bullet_list_.at(i);
        if (p_bullet != NULL)
        {
            if (p_bullet->get_is_move() == true)
            {
                p_bullet->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
                p_bullet->Render(des);
            }
            // khi get_is_move =false
            else
            {
                p_bullet_list_.erase(p_bullet_list_.begin() + i);
                // có erase để xóa rồi nma trong trường hợp nó chỉ cho vào danh sách xóa thì ta vẫn phải sd delete cho chắc
                if (p_bullet != NULL)
                {

                    delete p_bullet;
                    p_bullet = NULL;
                }
            }
        }
    }
}

void MainObject ::RemoveBullet(const int &idx)
{
    int size = p_bullet_list_.size();
    if (size > 0 && idx < size)
    {
        BulletObject *p_bullet = p_bullet_list_.at(idx);
        p_bullet_list_.erase(p_bullet_list_.begin() + idx);

        if (p_bullet)
        {
            delete p_bullet;
            p_bullet = NULL;
        }
    }
}

void MainObject::DoPlayer(Map &map_data)
{
    // khi come_back_time khác 0 tức là nhân vật đang không nằm trong bản đồ thì các xử lí của hàm doplayer không được sử dụng
    if (come_back_time_ == 0)
    {
        // dau tien x_val_=0 va y_val_ liên tục cộng thêm 0.8 để nhân vật rơi từ từ xuống
        x_val_ = 0;
        y_val_ += 0.8;
        // MAX_FALL_SPEED LÀ TỐC ĐỘ RƠI TỰ DO TỐI ĐA
        if (y_val_ >= MAX_FALL_SPEED)
        {
            y_val_ = MAX_FALL_SPEED;
        }

        if (input_type_.left_ == 1)
        {
            x_val_ -= PLAYER_SPEED;
        }
        else if (input_type_.right_ == 1)
        {
            x_val_ += PLAYER_SPEED;
        }

        if (input_type_.jump_ == 1)
        {
            // chi khi nao con dang tren mawjt dat thi moi cho nhay
            if (on_ground_ == true)
            {
                // không trừ bằng do nếu trừ bằng thì nó sẽ từ từ đi lên còn cho giá trị như này nó sẽ lên luôn
                y_val_ = -PLAYER_JUMP_VAL;
            }
            on_ground_ = false;
            // sau khi nhảy xong phải gán về 0
            input_type_.jump_ = 0;
        }

        CheckToMap(map_data);
        
        CenterEntityOnMap(map_data);
        
    }

    if (come_back_time_ > 0)
    {
        come_back_time_--;
        if (come_back_time_ == 0)
        {
            on_ground_ = false;
            if (x_pos_ > 512)
            {
                x_pos_ -= 256; // 4 tile map
            }
            else // nếu vị trí nhân vật đang ở rất gần phía đầu bản đồ
            {
                x_pos_ -=64;
            }
            // x_pos chính là vị trí nhân vật rơi xuống vực và xuất hiện trở lại
            y_pos_ = 0;
            x_val_ = 0;
            y_val_ = 0;
            Mix_PlayChannel(-1,g_revive,0);
        }
    }
}

void MainObject::CenterEntityOnMap(Map &map_data)
{
    // start_x_ là vị trí đầu tiên của map
    //  tính toán khi nhân vật di chuyển đến một nửa của bản đồ thì bản đồ sẽ được cuốn theo
    map_data.start_x_ = x_pos_ - (SCREEN_WIDTH / 2);
    // nếu bản đồ lùi thì không cho phép lùi
    if (map_data.start_x_ < 0)
    {
        map_data.start_x_ = 0;
    }
    else if (map_data.start_x_ + SCREEN_WIDTH >= map_data.max_x_)
    {
        map_data.start_x_ = map_data.max_x_ - SCREEN_WIDTH;
    }
    map_data.start_y_ = y_pos_ - (SCREEN_HEIGHT / 2);

    if (map_data.start_y_ < 0)
    {
        map_data.start_y_ = 0;
    }
    else if (map_data.start_y_ + SCREEN_HEIGHT >= map_data.max_y_)
    {
        map_data.start_y_ = map_data.max_y_ - SCREEN_HEIGHT;
    }
}

// đây là hàm quan trọng và là hàm chính kiểm tra va chạm của nhân vật với map bản đồ
void MainObject::CheckToMap(Map &map_data)
{
    // giới hạn kiểm tra từ a đến b theo chiều x
    int x1 = 0;
    int x2 = 0;

    // giới hạn kiểm tra từ a đến b theo chiều y
    int y1 = 0;
    int y2 = 0;

    // kiểm tra theo chiều ngang horizontal
    //  chiều cao min của height frame hoặc tile size cái nào bé hơn thì lấy
    int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;
    // tìm vị trí ô của nhân vật là x1
    x1 = (x_pos_ + x_val_) / TILE_SIZE;
    // trừ một là trừ đi lượng sai số để ví dụ nếu phép chia hết thì nó đúng bằng vị trí của phần từ map va chạm
    x2 = (x_pos_ + x_val_ + width_frame_ - 1) / TILE_SIZE;

    y1 = (y_pos_) / TILE_SIZE;
    y2 = (y_pos_ + height_min - 1) / TILE_SIZE;

    // kiem tra x1 y1 co nam trong vi tri gioi han cua ban do khong
    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if (x_val_ > 0)
        {
            int val1 = map_data.tile[y1][x2];
            int val2 = map_data.tile[y2][x2];

            if (val1 == STATE_MONEY && val2 == STATE_MONEY)
            {
                if (y1 != y2)
                {
                    map_data.tile[y1][x2] = 0;
                    map_data.tile[y2][x2] = 0;
                    IncreaseMoney();
                    IncreaseMoney();
                }
                else
                {
                    map_data.tile[y2][x2] = 0;
                    IncreaseMoney();
                }
            }
            else if (val1 == STATE_MONEY && val2 != STATE_MONEY)
            {
                map_data.tile[y1][x2] = 0;
                IncreaseMoney();
            }
            else if (val1 != STATE_MONEY && val2 == STATE_MONEY)
            {
                map_data.tile[y2][x2] = 0;
                IncreaseMoney();
            }
            else
            {

                // x_val >0 tức là đang di chuyển về bên phải

                // nếu trạng thái bản đồ khác 0 tức là có vật cản
                if (val1 != BLANK_TILE || val2 != BLANK_TILE)
                {
                    x_pos_ = x2 * TILE_SIZE;
                    x_pos_ -= width_frame_ + 1;
                    x_val_ = 0;
                }
            }
        }
        // trường hợp đi lùi
        else if (x_val_ < 0)
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y2][x1];

            if (val1 == STATE_MONEY && val2 == STATE_MONEY)
            {
                if (y1 != y2)
                {
                    map_data.tile[y1][x1] = 0;
                    map_data.tile[y2][x1] = 0;
                    IncreaseMoney();
                    IncreaseMoney();
                }
                else
                {
                    map_data.tile[y2][x1] = 0;
                    IncreaseMoney();
                }
            }
            else if (val1 == STATE_MONEY && val2 != STATE_MONEY)
            {
                map_data.tile[y1][x1] = 0;
                IncreaseMoney();
            }
            else if (val1 != STATE_MONEY && val2 == STATE_MONEY)
            {
                map_data.tile[y2][x1] = 0;
                IncreaseMoney();
            }
            else
            {
                if (val1 != BLANK_TILE || val2 != BLANK_TILE)
                {
                    x_pos_ = (x1 + 1) * TILE_SIZE;
                    x_val_ = 0;
                }
            }
        }
    }
    // check vertical
    int width_min = width_frame_ < TILE_SIZE ? width_frame_ : TILE_SIZE;
    x1 = (x_pos_) / TILE_SIZE;
    x2 = (x_pos_ + width_min) / TILE_SIZE;

    y1 = (y_pos_ + y_val_) / TILE_SIZE;
    y2 = (y_pos_ + y_val_ + height_frame_ - 1) / TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if (y_val_ > 0) // đây là trạng thái rơi tự do
        {
            int val1 = map_data.tile[y2][x1];
            int val2 = map_data.tile[y2][x2];

            if (val1 == STATE_MONEY && val2 == STATE_MONEY)
            {
                if (x1 != x2)
                {
                    map_data.tile[y2][x1] = 0;
                    map_data.tile[y2][x2] = 0;
                    IncreaseMoney();
                    IncreaseMoney();
                }
                else
                {
                    map_data.tile[y2][x2] = 0;
                    IncreaseMoney();
                }
            }
            else if (val1 == STATE_MONEY && val2 != STATE_MONEY)
            {
                map_data.tile[y2][x1] = 0;
                IncreaseMoney();
            }
            else if (val1 != STATE_MONEY && val2 == STATE_MONEY)
            {
                map_data.tile[y2][x2] = 0;
                IncreaseMoney();
            }
            else
            {
                if (val1 != BLANK_TILE || val2 != BLANK_TILE)
                {
                    y_pos_ = y2 * TILE_SIZE;
                    y_pos_ -= (height_frame_ + 1);
                    y_val_ = 0;
                    on_ground_ = true;
                    if (status_ == WALK_NONE)
                    {
                        status_ = WALK_RIGHT;
                    }
                }
            }
        }
        // nhan vat nhay len tren
        else if (y_val_ < 0)
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y1][x2];
            if (val1 == STATE_MONEY && val2 == STATE_MONEY)
            {
                if (x1 != x2)
                {
                    map_data.tile[y1][x1] = 0;
                    map_data.tile[y1][x2] = 0;
                    IncreaseMoney();
                    IncreaseMoney();
                }
                else
                {
                    map_data.tile[y1][x1] = 0;
                    IncreaseMoney();
                }
            }
            else if (val1 == STATE_MONEY && val2 != STATE_MONEY)
            {
                map_data.tile[y1][x1] = 0;
                IncreaseMoney();
            }
            else if (val1 != STATE_MONEY && val2 == STATE_MONEY)
            {
                map_data.tile[y1][x2] = 0;
                IncreaseMoney();
            }
            else
            {
                if (val1 != BLANK_TILE || val2 != BLANK_TILE)
                {
                    y_pos_ = (y1 + 1) * TILE_SIZE;
                    y_val_ = 0;
                }
            }
        }
    }

    // nếu các yếu tố va chạm không xảy ra
    x_pos_ += x_val_;
    y_pos_ += y_val_;

    if (x_pos_ < 0)
    {
        x_pos_ = 0;
    }
    else if (x_pos_ + width_frame_ > map_data.max_x_)
    {
        x_pos_ = map_data.max_x_ - width_frame_ - 1; // trừ đi 1 là trừ đi sai số
    }

    if (y_pos_ > map_data.max_y_)
    {
        // độ trễ là 60 để nó quay trở lại
        come_back_time_ = 60;
    }
}

void MainObject ::IncreaseMoney()
{
    money_count++;
    Mix_Chunk *beep_sound = Mix_LoadWAV(kSoundBeep);
    if (beep_sound != NULL)
    {
        Mix_PlayChannel(-1, beep_sound, 0);
    }
}

void MainObject::UpdateImagePlayer(SDL_Renderer *des)
{
    // hiện tại xử lí 2 trường hợp chính là đi và nhảy
    if (on_ground_ == true)
    {
        if (status_ == WALK_LEFT)
        {
            LoadImg("image//player_left.png", des);
        }
        else
        {
            LoadImg("image//player_right.png", des);
        }
    }
    else
    {
        if (status_ == WALK_LEFT)
        {
            LoadImg("image//jum_left.png", des);
        }
        else
        {
            LoadImg("image//jum_right.png", des);
        }
    }
}