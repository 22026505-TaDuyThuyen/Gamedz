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

BaseObject g_background;

#define THREAT_GRAVITY_SPEED 0.8
#define THREAT_FRAME_NUM 8
#define THREAT_MAX_FALL_SPEED 10
#define THREAT_SPEED 3
#define DINO_SPEED 10
class ThreatsObject : public BaseObject
{
public:
    ThreatsObject();
    ~ThreatsObject();

    enum TypeMove
    {
        // loại đang hiện có
        STATIC_THREAT = 0,
        // trong một phạm vi
        MOVE_IN_SPACE_THREAT = 1,
        // khủng lomg
        DINOSAUR_THREAT = 2,
    };

    // giá trị set của x_val_ và y thì chỉ cần truyền vào chứ không cần lấy ra
    void set_x_val(const float &xVal) { x_val_ = xVal; }
    void set_y_val(const float &yVal) { y_val_ = yVal; }

    void set_x_pos(const float &xp) { x_pos_ = xp; }
    void set_y_pos(const float &yp) { y_pos_ = yp; }
    float get_x_pos() const { return x_pos_; }
    float get_y_pos() const { return y_pos_; }
    void SetMapXY(const int &mp_x, const int &mp_y)
    {
        map_x_ = mp_x;
        map_y_ = mp_y;
    }

    // hàm truyền sử lí tạo các hiệu ứng clip
    void set_clips();
    bool LoadImg(std ::string path, SDL_Renderer *screen);
    void Show(SDL_Renderer *des);
    // hai hàm get sau đây sẽ cần trong bài toán check va chạm
    int get_width_frame() const { return width_frame_; }
    int get_height_frame() const { return height_frame_; }

    // hàm xử lí nhân vật di chuyển hay đứng yên hay va chạm với bản đồ
    void DoPlayer(Map &gMap);
    void CheckToMap(Map &gMap);
    // ở đây không cần centerentityomap vì map chỉ kéo theo khi ta cần di chuyển player thôi

    // hàm khởi tạo ban đầu cho quái sau khi rơi xuống vực
    void InitReturn();
    void set_type_move(const int &typeMove) { type_move_ = typeMove; }
    // hàm xét vị trí để di chuyển
    void SetAnimationPos(const int &pos_a, const int &pos_b) { animation_a_ = pos_a, animation_b_ = pos_b; }
    // ham dat trang thai cho quai
    void set_input_left(const int &ipLeft) { input_type_.left_ = ipLeft; }
    // ham xu li di chuyen trong pham vi a b
    void ImpMoveType(SDL_Renderer *screen);

    SDL_Rect GetRectFrame();
    std::vector<BulletObject *> get_bullet_list() const { return bullet_list_; }
    void set_bullet_list(const std::vector<BulletObject *> &bl_list) { bullet_list_ = bl_list; }

    void InitBullet(BulletObject *p_bullet, SDL_Renderer *screen);
    void MakeBullet(SDL_Renderer *screen, const int &x_limit, const int &y_limit);

    void RemoveBullet(const int &idx);

private:
    // 2 biến xác định giới hạn của quái với bản đồ để khi rơi xuống vực thẳm còn check
    int map_x_;
    int map_y_;

    // 2 biến tăng giá trị
    float x_val_;
    float y_val_;
    // vị trí hiện tại là vị trí bao nhiêu
    float x_pos_;
    float y_pos_;
    // nằm trên mặt đất hay chưa
    bool on_ground_;
    // thời gian trở lại khi rơi xuống vực
    int come_back_time_;
    // vẽ hình chữ nhật
    SDL_Rect frame_clip_[THREAT_FRAME_NUM];
    // chuyên quản lí các chỉ số của frame:
    int frame_;
    // kích thước của frame:
    int width_frame_;
    int height_frame_;

    // kiểu di chuyển
    int type_move_;
    // giới hạn a,b trong đó a là bên tay trái, b là bên tay phải ; quái di chuyển trong phạm vi từ a đến b
    int animation_a_;
    int animation_b_;
    // biến quản lí việc di chuyển
    Input input_type_;

    std ::vector<BulletObject *> bullet_list_;
};

ThreatsObject ::ThreatsObject()
{
    width_frame_ = 0;
    height_frame_ = 0;
    x_val_ = 0;
    y_val_ = 0;
    x_pos_ = 0.0;
    y_pos_ = 0.0;
    on_ground_ = 0;
    come_back_time_ = 0;
    frame_ = 0;

    animation_a_ = 0;
    animation_b_ = 0;
    input_type_.left_ = 0;
    type_move_ = STATIC_THREAT;
}

ThreatsObject ::~ThreatsObject()
{
}

bool ThreatsObject ::LoadImg(std ::string path, SDL_Renderer *screen)
{
    bool ret = BaseObject ::LoadImg(path, screen);
    if (ret == true)
    {
        width_frame_ = rect_.w / THREAT_FRAME_NUM;
        height_frame_ = rect_.h;
    }

    return ret;
}

SDL_Rect ThreatsObject ::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;

    return rect;
}

// bắt đầu xây dựng clip cho frame clip
void ThreatsObject ::set_clips()
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

void ThreatsObject ::Show(SDL_Renderer *des)
{
    if (come_back_time_ == 0)
    {
        rect_.x = x_pos_ - map_x_;
        rect_.y = y_pos_ - map_y_;
        frame_++;
        if (frame_ > 8)
        {
            frame_ = 0;
        }
        SDL_Rect *currentClip = &frame_clip_[frame_];
        SDL_Rect rendQuad = {rect_.x, rect_.y, width_frame_, height_frame_};
        SDL_RenderCopy(des, p_object_, currentClip, &rendQuad);
    }
}

void ThreatsObject ::DoPlayer(Map &gMap)
{
    if (come_back_time_ == 0)
    {
        x_val_ = 0;
        y_val_ += THREAT_GRAVITY_SPEED;
        if (y_val_ >= THREAT_MAX_FALL_SPEED)
        {
            y_val_ = THREAT_MAX_FALL_SPEED;
        }

        if (input_type_.left_ == 1 && type_move_ == MOVE_IN_SPACE_THREAT)
        {
            x_val_ -= THREAT_SPEED;
        }
        else if (input_type_.right_ == 1 && type_move_ ==  MOVE_IN_SPACE_THREAT)
        {
            x_val_ += THREAT_SPEED;
        }
        else if(input_type_.left_ == 1 && type_move_ == DINOSAUR_THREAT)
        {
            x_val_ -= DINO_SPEED;
        }
        else if(input_type_.right_ == 1 && type_move_ ==  DINOSAUR_THREAT)
        {
            x_val_ += DINO_SPEED;
        }
        CheckToMap(gMap);
    }
    else if (come_back_time_ > 0)
    {
        come_back_time_--;
        if (come_back_time_ == 0)
        {
            InitReturn();
        }
    }
}
void ThreatsObject ::RemoveBullet(const int &idx)
{
    int size = bullet_list_.size();
    if (size > 0 && idx < size)
    {
        BulletObject *p_bullet = bullet_list_.at(idx);
        bullet_list_.erase(bullet_list_.begin() + idx);

        if (p_bullet)
        {
            delete p_bullet;
            p_bullet = NULL;
        }
    }
}
void ThreatsObject ::InitReturn()
{
    x_val_ = 0;
    y_val_ = 0;
    if (x_pos_ > 256)
    {
        x_pos_ -= 256;
        animation_a_ -= 256;
        animation_b_ -= 256;
    }
    else
    {
        x_pos_ = 0;
    }
    y_pos_ = 0;
    come_back_time_ = 0;
    input_type_.left_ = 1;
}
// đây là hàm quan trọng và là hàm chính kiểm tra va chạm của nhân vật với map bản đồ
void ThreatsObject::CheckToMap(Map &map_data)
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

            // x_val >0 tức là đang di chuyển về bên phải

            // nếu trạng thái bản đồ khác 0 tức là có vật cản
            if ((val1 != BLANK_TILE && val1 != STATE_MONEY) || (val2 != BLANK_TILE && val2 != STATE_MONEY))
            {
                x_pos_ = x2 * TILE_SIZE;
                x_pos_ -= width_frame_ + 1;
                x_val_ = 0;
            }
        }
        // trường hợp đi lùi
        else if (x_val_ < 0)
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y2][x1];

            if ((val1 != BLANK_TILE && val1 != STATE_MONEY) || (val2 != BLANK_TILE && val2 != STATE_MONEY))
            {
                x_pos_ = (x1 + 1) * TILE_SIZE;
                x_val_ = 0;
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

            if ((val1 != BLANK_TILE && val1 != STATE_MONEY) || (val2 != BLANK_TILE && val2 != STATE_MONEY))
            {
                y_pos_ = y2 * TILE_SIZE;
                y_pos_ -= (height_frame_ + 1);
                y_val_ = 0;
                on_ground_ = true;
            }
        }
        // nhan vat nhay len tren
        else if (y_val_ < 0)
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y1][x2];

            if ((val1 != BLANK_TILE && val1 != STATE_MONEY) || (val2 != BLANK_TILE && val2 != STATE_MONEY))
            {
                y_pos_ = (y1 + 1) * TILE_SIZE;
                y_val_ = 0;
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
    // bài 7 xử lí khi nhân vật rơi xuống vực
    if (y_pos_ > map_data.max_y_)
    {
        // độ trễ là 60 để nó quay trở lại
        come_back_time_ = 60;
    }
}

void ThreatsObject ::ImpMoveType(SDL_Renderer *screen)
{
    if (type_move_ == STATIC_THREAT)
    {
    }
    else if (type_move_ == DINOSAUR_THREAT)
    {
        if (on_ground_ == true)
        {
            if (x_pos_ > animation_b_)
            {
                input_type_.left_ = 1;
                input_type_.right_ = 0;
                LoadImg("image//diano_left.png", screen);
            }
            else if (x_pos_ < animation_a_)
            {
                input_type_.left_ = 0;
                input_type_.right_ = 1;
                LoadImg("image//diano_right.png", screen);
            }
        }
        // trường hợp trong không trung
        else
        {
            if (input_type_.left_ == 1)
            {
                LoadImg("image//diano_left.png", screen);
            }
        }
    }
    else if (type_move_ == MOVE_IN_SPACE_THREAT)
    {
        if (on_ground_ == true)
        {
            if (x_pos_ > animation_b_)
            {
                input_type_.left_ = 1;
                input_type_.right_ = 0;
                LoadImg("image//threat_left.png", screen);
            }
            else if (x_pos_ < animation_a_)
            {
                input_type_.left_ = 0;
                input_type_.right_ = 1;
                LoadImg("image//threat_right.png", screen);
            }
        }
        // trường hợp trong không trung
        else
        {
            if (input_type_.left_ == 1)
            {
                LoadImg("image//threat_left.png", screen);
            }
        }
    }
}

void ThreatsObject::InitBullet(BulletObject *p_bullet, SDL_Renderer *screen)
{
    if (p_bullet != NULL)
    {
        p_bullet->set_bullet_type(BulletObject::LASER_BULLET);
        bool ret = p_bullet->LoadImgBullet(screen);
        if (ret)
        {
            p_bullet->set_is_move(true);
            p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
            int xp = x_pos_ - map_x_;
            int yp = y_pos_ - map_y_;

            p_bullet->SetRect(xp + 5, yp + 10);
            // xet toc do
            p_bullet->set_x_val(15);
            bullet_list_.push_back(p_bullet);
        }
    }
}

void ThreatsObject ::MakeBullet(SDL_Renderer *screen, const int &x_limit, const int &y_limit)
{
    for (int i = 0; i < bullet_list_.size(); i++)
    {
        BulletObject *p_bullet = bullet_list_.at(i);
        if (p_bullet != NULL)
        {
            if (p_bullet->get_is_move())
            {
                int bullet_distance = rect_.x + width_frame_ - p_bullet->GetRect().x;
                if (bullet_distance < 300 && bullet_distance > 0)
                {
                    p_bullet->HandleMove(x_limit, y_limit);
                    // sau khi xử lí cho đạn bắn bằng HandleBullet thì hiển thị nó trên màn hình bằng Render
                    p_bullet->Render(screen);
                }
                else
                {
                    p_bullet->set_is_move(false);
                }
            }
            else
            {
                p_bullet->set_is_move(true);
                p_bullet->SetRect(rect_.x + 5, rect_.y + 10);
            }
        }
    }
}

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
TTF_Font *font_time = NULL;

class TextObject : public BaseObject
{
public:
    TextObject();
    ~TextObject();

    enum TextColor
    {
        RED_TEXT = 0,
        WHITE_TEXT = 1,
        BLACK_TEXT = 2,
        YELLOW_TEXT = 3,

    };

    // hàm có nhiệm vụ có đầu vào là font chữ
    bool LoadFromRenderText(TTF_Font *font, SDL_Renderer *screen);
    void Free();
    // hàm đầu tiên truyền trực tiếp giá trị màu vào
    void SetColor(Uint8 red, Uint8 green, Uint8 blue);
    // hàm xét những màu cơ bản của ta
    void SetColor(int type);
    // hàm show text lên màn hình
    void RenderText(SDL_Renderer *screen,
                    int xp, int yp,                         // vị trí
                    SDL_Rect *clip = NULL,                  // có cần clip không (nếu là hiệu ứng ảnh động thì có cần)
                    double angle = 0.0,                     // góc nghiêng
                    SDL_Point *center = NULL,               // SDL_Point sử dụng để biểu diễn tọa độ của điểm trung tâm trên màn hình
                    SDL_RendererFlip flip = SDL_FLIP_NONE); // xác định cách thức hiển thị một đối tượng lên màn hình (lật ngang hay dọc)
                                                            // lấy ra chiều rộng
    int GetWidth() const { return width_; }
    // lấy ra chiều cao
    int GetHeight() const { return height_; }

    //  truyền vào nội dung của text là gì
    void SetText(const std::string &text) { str_val_ = text; }
    // trả ra nội dung của text khi cần
    std::string GetText() const { return str_val_; }
    void SetRech(int x, int y )
    {
        rect_.x = x;
        rect_.y = y;
        rect_.w = width_;
        rect_.h = height_;
    }
    SDL_Rect GetRech(){return rect_;}

private:
    // lưu nội dung của text
    std::string str_val_;
    SDL_Color text_color_;
    SDL_Texture *texture_;
    SDL_Rect rect_;
    int width_;
    int height_;
};

TextObject ::TextObject()
{
    text_color_.r = 255;
    text_color_.g = 255;
    text_color_.b = 255;
    texture_ = NULL;
}

TextObject ::~TextObject()
{
}

bool TextObject::LoadFromRenderText(TTF_Font *font, SDL_Renderer *screen)
{
    // đầu tiên phải tạo ra hình ảnh chữ
    SDL_Surface *text_surface = TTF_RenderText_Solid(font, str_val_.c_str(), text_color_);
    if (text_surface)
    {
        texture_ = SDL_CreateTextureFromSurface(screen, text_surface);
        // lấy ra chiều rộng
        width_ = text_surface->w;

        height_ = text_surface->h;

        SDL_FreeSurface(text_surface);
    }

    return texture_ != NULL;
    // != NULL thì trả về true
}
void TextObject ::Free()
{
    if (texture_ != NULL)
    {
        SDL_DestroyTexture(texture_);
        texture_ = NULL;
    }
}

void TextObject ::SetColor(Uint8 red, Uint8 green, Uint8 blue)
{
    text_color_.r = red;
    text_color_.g = green;
    text_color_.b = blue;
}

void TextObject ::SetColor(int type)
{
    if (type == RED_TEXT)
    {
        SDL_Color color = {255, 0, 0};
        text_color_ = color;
    }
    else if (type == WHITE_TEXT)
    {
        SDL_Color color = {255, 255, 255};
        text_color_ = color;
    }
    else if (type == BLACK_TEXT)
    {
        SDL_Color color = {0, 0, 0};
        text_color_ = color;
    }
    else if (type == YELLOW_TEXT)
    {
        SDL_Color color = {248,250,28};
        text_color_ = color;
    }
}
void TextObject::RenderText(SDL_Renderer *screen,
    int xp, int yp, SDL_Rect *clip /* = NULL */,
    double angle /* = 0.0 */, SDL_Point *center /* = NULL */,
    SDL_RendererFlip flip /* = SDL_FLIP_NONE */)
{
    SDL_Rect renderQuad = {xp, yp, width_, height_};
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopyEx(screen, texture_, clip, &renderQuad, angle, center, flip);
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
BaseObject g_menu ;
// bool LoadMenu()
// {
//     bool ret = g_menu.LoadImg("image//menu.png",g_screen);
//     if(ret == false)
//     {
//         return false;
//     }
//     return true;
// }
BaseObject g_game_over;
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
    // lấy tọa độ của con chuột
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
                        // SDL_RenderClear(g_screen);
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
                    // Mix_PlayChannel(-1,g_click,0);
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
TextObject mark_game;
UINT mark_value = 0;
int ShowGameOver(SDL_Event g_event,bool is_win)
{
    const int Item =2 ;
    SDL_Rect pos_arr[Item];

    pos_arr[0].x = SCREEN_WIDTH*0.41;
    pos_arr[0].y = SCREEN_HEIGHT*0.85;
    pos_arr[1].x = SCREEN_WIDTH*0.41;
    pos_arr[1].y = SCREEN_HEIGHT*0.9;

    TextObject  text_menu[Item];

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

// int ShowWinner(SDL_Event g_event)
// {
//     g_game_over.LoadImg("image//win.png",g_screen);
//     g_game_over.Render(g_screen);

//     const int Item =2 ;
//     SDL_Rect pos_arr[Item];
//     pos_arr[0].x = SCREEN_WIDTH*0.43;
//     pos_arr[0].y = SCREEN_HEIGHT*0.6;
//     pos_arr[1].x = SCREEN_WIDTH*0.43;
//     pos_arr[1].y = SCREEN_HEIGHT*0.65;

//     TextObject  text_menu[Item];
//     text_menu[0].SetText("YOUR POINTS: ");
//     text_menu[0].SetColor(TextObject :: YELLOW_TEXT);
//     text_menu[0].LoadFromRenderText(font_time,g_screen);
//     text_menu[0].RenderText(g_screen, pos_arr[0].x , pos_arr[0].y);
//     text_menu[0].SetRech(pos_arr[0].x , pos_arr[0].y);
    

//     text_menu[1].SetText("EXIT");
//     text_menu[1].SetColor(TextObject :: YELLOW_TEXT);
//     text_menu[1].LoadFromRenderText(font_time , g_screen);
//     text_menu[1].RenderText(g_screen , pos_arr[1].x , pos_arr[1].y);
//     text_menu[1].SetRech(pos_arr[1].x , pos_arr[1].y);
    
//     bool selected = {0};
//     // lấy tọa độ của con chuột
//     int xm = 0;
//     int ym = 0;

//     while (true)
//     {
//         while(SDL_PollEvent(&g_event))
//         {
//             switch(g_event.type)
//             {
//                 case SDL_QUIT :
//                     return 1;
//                 case SDL_MOUSEMOTION:
//                 {
//                     xm = g_event.motion.x;
//                     ym = g_event.motion.y;
//                     // SDL_RenderClear(g_screen);
//                     if(checkpositionforrect(xm , ym , text_menu[1].GetRech()))
//                     {
//                         if(selected== false)
//                         {
//                             selected= 1;
//                             text_menu[1].SetColor(TextObject::RED_TEXT);
//                             text_menu[1].LoadFromRenderText(font_time , g_screen);
//                             text_menu[1].RenderText(g_screen, pos_arr[1].x , pos_arr[1].y);
//                         }
//                     }
//                     else 
//                     {
//                         if(selected==true)
//                         {
//                             selected=0;
//                             text_menu[1].SetColor(TextObject::YELLOW_TEXT);
//                             text_menu[1].LoadFromRenderText(font_time , g_screen);
//                             text_menu[1].RenderText(g_screen, pos_arr[1].x , pos_arr[1].y);
//                         }
//                     }
                    
//                 }
//                 break;
//                 case SDL_MOUSEBUTTONDOWN:
//                 {
//                     xm = g_event.button.x;
//                     ym = g_event.button.y;
                    
//                     if(checkpositionforrect(xm , ym , text_menu[1].GetRech()))
//                     {
//                         return 1;
//                     }
            
//                 }
//                 break; 
//                 case SDL_KEYDOWN:
//                 if(g_event.key.keysym.sym == SDLK_ESCAPE)
//                 {
//                     return 1;
//                 }
//                 default: 
//                 break;
//             }

//         }
//         std ::string mark_game_over = std :: to_string(mark_value);
//         mark_game.SetText(mark_game_over);
//         mark_game.SetColor(TextObject :: YELLOW_TEXT);
//         mark_game.LoadFromRenderText(font_time, g_screen);
//         mark_game.RenderText(g_screen, SCREEN_WIDTH*0.6, SCREEN_HEIGHT*0.6);

//         SDL_RenderPresent(g_screen);
//     }  
//     return 1;
// }

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
    // gọi hàm set_clip nếu thiếu thì sẽ không tạo ra hiệu ứng
    p_player.set_clips();

    PlayerPower player_power;
    player_power.Init(g_screen);

    PlayerMoney player_money;
    player_money.Init(g_screen);
    player_money.SetPos(SCREEN_WIDTH * 0.5 - 350, 8);
    

    std ::vector<ThreatsObject *> threats_list = MakeThreatList();

    // BOSS THREAT
    BossObject bossObject;
   // bool ret = bossObject.LoadImg("image//boss_object.png", g_screen);
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
    

    // TEXT VỀ THỜI GIAN
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

        // cho vào game map .set map để bản đồ cập nhật vị trí mới cho start x và start y;
        // sau khi vào trong doplayer thì map_data thay đổi nên cần có hàm SetMap để set lại các giá trị của start_x_ và start_y_
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
                        //money_count-=15;
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
                    // else 
                    // {
                    //     quit =false;
                    //     num_boss_die = 0;
                    //     num_die = 0;
                    //     p_player.SetRect(0,0);
                    //     p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
                    //     p_player.DoPlayer(map_data);
                    // }

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
        // TẠO ĐẠN BẮN CON BOSS
      //  bossObject.Show(g_screen);
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
                        //money_count-=15;
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
                    // else 
                    // {
                    //     quit =false;
                    //     num_boss_die = 0;
                    //     num_die = 0;
                    //     p_player.SetRect(0,0);
                    //     p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
                    //     p_player.DoPlayer(map_data);
                    // }

                }      
            }
        }

        // SHOW GAME TIME
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
            // truyền vào nội dung là gì
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

        // SHOW BOSS
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
