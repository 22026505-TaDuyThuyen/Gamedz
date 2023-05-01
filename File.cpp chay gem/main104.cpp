// FIX LẠI BUG ĐỒNG TIỀN
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

static char g_name_audio_bullet_main1[] = {"sound//Fire.wav"};
static char g_name_audio_bullet_main2[] = {"sound//Laser.wav"};
static char g_name_audio_ex_main[] = {"sound//Explosion+1.wav"};
static char g_name_audio_ex_threats[] = {"sound//Bomb1.wav"};
static char g_name_sound_increase[] = {"sound//two_beep_.wav"};
static char kSoundBeep[] = {"sound//beep_.wav"};

const int FRAME_PER_SECOND = 25;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int RENDER_DRAW_COLOR = 0Xff;

// 4.1
// định nghĩa trạng thái bằng 0
#define BLANK_TILE 0
// b3 define thi ko co ; o cuoi
#define TILE_SIZE 64
#define MAX_MAP_X 400 // full la 400 cot
#define MAX_MAP_Y 10  // full la 400 dong
// bai 4

#define STATE_MONEY 4

// cau truc cho viec di chuyen
typedef struct Input
{
    // quay lai
    int left_;
    int right_;
    // gio sung len tren
    int up_;
    // ngoi xuong, tro sung xuong
    int down_;
    // nhay
    int jump_;
};

// ket thuc bai 4
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

class BaseObject
{
public:
    BaseObject();
    ~BaseObject();
    void SetRect(const int &x, const int &y) { rect_.x = x, rect_.y = y; } // ham set kich thuoc cho rect
    SDL_Rect GetRect() const { return rect_; }
    SDL_Texture *GetObject() const { return p_object_; } // ham lay ra p_object_

    // ham load anh bitmap len tren doi tuong p_object_ sau do dung p_object_ dua len man hinh
    virtual bool LoadImg(std::string path, SDL_Renderer *screen);
    void Render(SDL_Renderer *des, const SDL_Rect *clip = NULL);
    void free();
  //menu  void Show(SDL_Surface*des);

protected:
    SDL_Texture *p_object_; // luu tru hinh anh;
    SDL_Rect rect_;         // luu kich thuoc;
};
BaseObject g_background;

BaseObject::BaseObject()
{
    p_object_ = NULL;
    rect_.x = 0;
    rect_.y = 0;
    rect_.w = 0;
    rect_.h = 0;
}
BaseObject::~BaseObject()
{
    free();
}
bool BaseObject::LoadImg(std::string path, SDL_Renderer *screen)
{
    free();
    SDL_Texture *new_texture = NULL;

    // bat dau qua trinh setup tam anh
    SDL_Surface *load_surface = IMG_Load(path.c_str());
    if (load_surface != NULL)
    {
        SDL_SetColorKey(load_surface, SDL_TRUE, SDL_MapRGB(load_surface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
        new_texture = SDL_CreateTextureFromSurface(screen, load_surface);
        if (new_texture != 0)
        {
            rect_.w = load_surface->w;
            rect_.h = load_surface->h;
            // newtexture dang luu giu toan bo thong tin tam anh
        }
        SDL_FreeSurface(load_surface);
    }
    p_object_ = new_texture;
    return p_object_ != NULL; // khac null return 1 ,bang null return 0;

    // doi tuong p_object_ sau ham loadimg nay da co thong tin cua texture va thong tin ve kich thuoc;
}
void BaseObject::Render(SDL_Renderer *des, const SDL_Rect *clip /*=NULL*/) // dua anh len SDL_Renderer*des
{
    SDL_Rect renderquad = {rect_.x, rect_.y, rect_.w, rect_.h}; // bao gom vi tri va kich thuoc tam anh
    SDL_RenderCopy(des, p_object_, clip, &renderquad);          // day toan bo thong so cua p_object_  len des voi kich thuoc va vi tri nam trong renderquad
}

// void BaseObject ::Show(SDL_Surface*des)
// {
//     if(p_object_ != NULL)
//     {
//         rect_ = SDL_RenderPresent(g_screen);
//     }
// }

void BaseObject::free()
{
    if (p_object_ != NULL)
    {
        SDL_DestroyTexture(p_object_);
        p_object_ = NULL;
        rect_.w = 0;
        rect_.h = 0;
    }
}
#define MAX_TILES 20

class TileMat : public BaseObject
{
public:
    TileMat() { ; }
    ~TileMat() { ; }
};

class GameMap
{
public:
    GameMap() { ; }
    ~GameMap() { ; }

    void LoadMap(char *name);
    void LoadTiles(SDL_Renderer *screen);
    void DrawMap(SDL_Renderer *screen);
    Map getMap() const { return game_map_; };
    void SetMap(Map &map_data) { game_map_ = map_data; };

private:
    Map game_map_;
    TileMat tile_mat[MAX_TILES];
};
void GameMap::LoadMap(char *name)
{
    FILE *fp = NULL;
    fopen_s(&fp, name, "rb");
    if (fp == NULL)
    {
        return;
    }

    game_map_.max_x_ = 0;
    game_map_.max_y_ = 0;

    for (int i = 0; i < MAX_MAP_Y; i++)
    {
        for (int j = 0; j < MAX_MAP_X; j++)
        {
            fscanf_s(fp, "%d", &game_map_.tile[i][j]);
            int val = game_map_.tile[i][j];
            if (val > 0)
            {
                if (j > game_map_.max_x_)
                {
                    game_map_.max_x_ = j;
                }

                if (i > game_map_.max_y_)
                {
                    game_map_.max_y_ = i;
                }
            }
        }
    }

    game_map_.max_x_ = (game_map_.max_x_ + 1) * TILE_SIZE;
    game_map_.max_y_ = (game_map_.max_y_ + 1) * TILE_SIZE;

    game_map_.start_x_ = 0;
    game_map_.start_y_ = 0;

    game_map_.file_name_ = name;
    fclose(fp);
}

void GameMap::LoadTiles(SDL_Renderer *screen)
{
    char file_img[30];
    FILE *fp = NULL;

    for (int i = 0; i < MAX_TILES; i++)
    {
        sprintf_s(file_img, "map/%d.png", i);

        fopen_s(&fp, file_img, "rb");
        if (fp == NULL)
        {
            continue;
        }

        fclose(fp);

        tile_mat[i].LoadImg(file_img, screen);
    }
}

void GameMap::DrawMap(SDL_Renderer *screen)
{
    int x1 = 0;
    int x2 = 0;

    int y1 = 0;
    int y2 = 0;

    int map_x = 0;
    int map_y = 0;

    map_x = game_map_.start_x_ / TILE_SIZE;
    x1 = (game_map_.start_x_ % TILE_SIZE) * -1;
    x2 = x1 + SCREEN_WIDTH + (x1 == 0 ? 0 : TILE_SIZE);

    map_y = game_map_.start_y_ / TILE_SIZE;

    y1 = (game_map_.start_y_ % TILE_SIZE) * -1;
    y2 = y1 + SCREEN_HEIGHT + (y1 == 0 ? 0 : TILE_SIZE);

    for (int i = y1; i < y2; i += TILE_SIZE)
    {
        map_x = game_map_.start_x_ / TILE_SIZE;
        for (int j = x1; j < x2; j += TILE_SIZE)
        {
            int val = game_map_.tile[map_y][map_x];
            if (val > 0)
            {
                tile_mat[val].SetRect(j, i);
                tile_mat[val].Render(screen);
            }
            map_x++;
        }
        map_y++;
    }
}
class BulletObject : public BaseObject
{
public:
    BulletObject();
    ~BulletObject();

    enum BulletDir
    {
        DIR_RIGHT = 20,
        DIR_LEFT = 21,
    };

    enum BulletType
    {

        SPHERE_BULLET = 50,
        LASER_BULLET = 51,
        BOSS_BULLET = 52,

    };

    void set_x_val(const int &xVal) { x_val_ = xVal; }
    void set_y_val(const int &yVal) { y_val_ = yVal; }
    int get_x_val() const { return x_val_; }
    int get_y_val() const { return y_val_; }

    void set_is_move(const bool &isMove) { is_move_ = isMove; }
    bool get_is_move() const { return is_move_; }

    void set_bullet_dir(const unsigned int &bulletDir) { bullet_dir_ = bulletDir; }
    unsigned int get_bullet_dir() const { return bullet_dir_; }

    void set_bullet_type(const unsigned int &bulletType) { bullet_type_ = bulletType; }
    unsigned int get_bullet_type() const { return bullet_type_; }

    // ham chuyen xu li cho van de di chuyen cua danj
    //  x_border và y_border là các giới hạn của màn hình để cho đạn được xóa đi
    void HandleMove(const int &x_border, const int &y_border);

    // hàm chuyển load hình ảnh
    bool LoadImgBullet(SDL_Renderer *des);

private:
    int x_val_;
    int y_val_;
    // neu nằm ngoài màn hình thì đạn sẽ bị xóa
    bool is_move_;
    // biến về hướng của đạn
    unsigned int bullet_dir_;

    // bien moi quan li loai dan
    unsigned int bullet_type_;
};

BulletObject ::BulletObject()
{
    x_val_ = 0;
    y_val_ = 0;
    is_move_ = false;
    bullet_type_ = SPHERE_BULLET;
}
BulletObject ::~BulletObject()
{
}

bool BulletObject ::LoadImgBullet(SDL_Renderer *des)
{
    bool ret = false;
    if (bullet_type_ == LASER_BULLET)
    {
        ret = LoadImg("image//laser_bullet.png", des);
    }
    else if (bullet_type_ == SPHERE_BULLET)
    {
        ret = LoadImg("image//sphere_bullet.png", des);
    }
    else if (bullet_type_ == BOSS_BULLET)
    {
        ret = LoadImg("image//boss bullet.png", des);
    }
    return ret;
}

void BulletObject ::HandleMove(const int &x_border, const int &y_border)
{
    if (bullet_dir_ == DIR_RIGHT)
    {
        rect_.x += x_val_;
        if (rect_.x > x_border)
        {
            is_move_ = false;
        }
    }
    else if (bullet_dir_ == DIR_LEFT)
    {
        rect_.x -= x_val_;

        if (rect_.x < 0) // chỉ nhỏ hơn 0 thôi chứ không phải border hay cgi hết

        {
            is_move_ = false;
        }
    }
}

// 4.1
// TỐC ĐỘ RƠI
#define GRAVITY_SPEED 0.8
// TỐC ĐỘ RƠI TỰ DO TỐI ĐA
#define MAX_FALL_SPEED 10
// TỐC ĐỘ DI CHUYỂN NHÂN VẬT
#define PLAYER_SPEED 8

#define PLAYER_JUMP_VAL 20

class MainObject : public BaseObject
{

public:
    MainObject();
    ~MainObject();

    enum WalkType
    {
        WALK_NONE = 0,
        WALK_RIGHT = 1,
        WALK_LEFT = 2,
    };

    // can co thong so rieng cho kich thuoc tam anh
    bool LoadImg(std::string path, SDL_Renderer *screen);
    // phai show theo tung frame
    void Show(SDL_Renderer *des);
    // ham xu li su kien khi bam trai phai di chuyen
    void HandelInputAction(SDL_Event events, SDL_Renderer *screen);
    // hàm chuyên sử lí các tạo hiệu ứng clip
    void set_clips();
    // hàm cộng trừ một lượng cho nhân vật di chuyển can truyen vao ban do de nhan vat khong di qua cho phep
    // 4.1
    // map_data là tham số của bản đồ
    void DoPlayer(Map &map_data);
    // kiem tra xem nhân vật đã va chạm vào map chưa để đứng được trên map
    void CheckToMap(Map &map_data);
    // hàm set vào 2 tham số của bản đồ
    // 4.2
    void SetMapXY(const int map_x, const int map_y)
    {
        map_x_ = map_x;
        map_y_ = map_y;
    }
    // hàm tính toán thông số của map khi di chuyển nhân vật
    void CenterEntityOnMap(Map &map_data);
    // hàm chuyên cho update các img khi nhảy một img khi chạy một img
    void UpdateImagePlayer(SDL_Renderer *des);

    SDL_Rect GetRectFrame();

    void set_bullet_list(std::vector<BulletObject *> bullet_list)
    {
        p_bullet_list_ = bullet_list;
    }
    std::vector<BulletObject *> get_bullet_list() const { return p_bullet_list_; }
    void HandleBullet(SDL_Renderer *des);
    void RemoveBullet(const int &idx);
    void IncreaseMoney();

    int GetMoneyCount() const { return money_count; }

    void set_comeback_time(const int &cb_time) { come_back_time_ = cb_time; }

    int get_frame_width() const { return width_frame_; }

    int get_frame_height() const { return height_frame_; }

private:
    int money_count;
    // tạo 1 vector để lưu trữ băng đạn
    std::vector<BulletObject *> p_bullet_list_;

    // bien tang len bao nhieu
    float x_val_;
    float y_val_;

    // vi tri nhan vat de show
    float x_pos_;
    float y_pos_;

    // kich thuoc tung frame nhan vat
    int width_frame_;
    int height_frame_;

    // luu cac frame: co 8 frame
    SDL_Rect frame_clip_[8];
    // bien ve trang thai
    Input input_type_;
    // bien luu chi so frame
    int frame_;
    // biến lưu trạng thái của hai kiểu di chuyển là sang phải hay sang trái;
    int status_;
    // trạng thái khi nhân vật đứng trên mặt đất r
    bool on_ground_;

    // 4.2 thêm 2 biến map_x và map_y để lưu vị trí của bản đồ
    int map_x_;
    int map_y_;

    // biến mới bài 7
    // thời gian chờ đợi xuất hiện trở lại
    int come_back_time_;
};

MainObject::MainObject()
{
    frame_ = 0;
    x_pos_ = 0;
    y_pos_ = 500;
    x_val_ = 0;
    y_val_ = 0;
    width_frame_ = 0;
    height_frame_ = 0;
    // chua biet status la ben trai hay ben phai
    status_ = WALK_NONE;
    // 4.1
    input_type_.left_ = 0;
    input_type_.right_ = 0;
    input_type_.jump_ = 0;
    input_type_.down_ = 0;
    input_type_.up_ = 0;
    // 4.1
    on_ground_ = false;
    // 4.2
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
            input_type_.left_ = 0; // 4.1
            // bai6
            UpdateImagePlayer(screen);
        }
        break;
        case SDLK_LEFT:
        {
            status_ = WALK_LEFT;
            input_type_.left_ = 1;
            input_type_.right_ = 0; // 4.1
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
            if (x_pos_ > 256)
            {
                x_pos_ -= 256; // 4 tile map
            }
            else // nếu vị trí nhân vật đang ở rất gần phía đầu bản đồ
            {
                x_pos_ = 0;
            }
            // x_pos chính là vị trí nhân vật rơi xuống vực và xuất hiện trở lại
            y_pos_ = 0;
            x_val_ = 0;
            y_val_ = 0;
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
    // bài 7 xử lí khi nhân vật rơi xuống vực
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

class ImpTimer
{
public:
    ImpTimer();
    ~ImpTimer();
    // hàm bắt đầu chạy sau đó đếm thời gian
    void start();
    void stop();
    void paused();
    void unpaused();

    int get_ticks();

    // hai hàm trả lại trạng thái là is_pause và is_start
    bool is_started();
    bool is_paused();

private:
    // thời điểm bắt đầu chạy là thời điểm nào
    int start_tick_;
    // thời điểm tạm dừng
    int paused_tick_;
    // trạng thái dừng hay bắt đầu
    bool is_paused_;
    bool is_started_;
};

ImpTimer::ImpTimer()
{
    start_tick_ = 0;
    paused_tick_ = 0;
    is_paused_ = false;
    is_started_ = false;
}

ImpTimer::~ImpTimer()
{
}

void ImpTimer::start()
{
    // bắt đầu chạy
    is_started_ = true;
    is_paused_ = false;

    start_tick_ = SDL_GetTicks();
}

void ImpTimer::stop()
{
    is_paused_ = false;
    is_started_ = false;
}

void ImpTimer ::paused()
{
    if (is_started_ == true && is_paused_ == false)
    {
        is_paused_ = true;
        // xem thời điểm pause là thời điểm nào bằng cách lấy thời điểm hiện tại trừ thời điểm đã trôi qua
        paused_tick_ = SDL_GetTicks() - start_tick_;
    }
}

void ImpTimer ::unpaused()
{
    if (is_paused_ == true)
    {
        is_paused_ = false;
        start_tick_ = SDL_GetTicks() - paused_tick_;
        paused_tick_ = 0;
    }
}
// xem thời gian tại thời điểm hiện tại là bao nhiêu
int ImpTimer ::get_ticks()
{
    if (is_started_ == true)
    {
        if (is_paused_ == true)
        {
            return paused_tick_;
        }
        else
        {
            return SDL_GetTicks() - start_tick_;
        }
    }
    return 0;
}

bool ImpTimer ::is_started()
{
    return is_started_;
}

bool ImpTimer ::is_paused()
{
    return is_paused_;
}

#define THREAT_GRAVITY_SPEED 0.8
#define THREAT_FRAME_NUM 8
#define THREAT_MAX_FALL_SPEED 10
#define THREAT_SPEED 3

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

        if (input_type_.left_ == 1)
        {
            x_val_ -= THREAT_SPEED;
        }
        else if (input_type_.right_ == 1)
        {
            x_val_ += THREAT_SPEED;
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
            p_threat->set_x_pos(25000+15*i);
            p_threat->set_y_pos(511);
            int pos1 = p_threat->get_x_pos() -700 -15*i ;
            int pos2 = 25088;
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

#define NUM_FRAME_EXP 8

class ExplosionObject : public BaseObject
{

public:
    ExplosionObject();
    ~ExplosionObject();

    void set_clips();
    void set_frame(const int &fr) { frame_ = fr; }
    virtual bool LoadImg(std::string path, SDL_Renderer *screen);
    void Show(SDL_Renderer *screen);
    int get_frame_width() const { return frame_width_; }
    int get_frame_height() const { return frame_height_; }

private:
    int frame_width_;
    int frame_height_;

    unsigned int frame_;
    SDL_Rect frame_clip_[8];
};

ExplosionObject ::ExplosionObject()
{
    frame_height_ = 0;
    frame_width_ = 0;
    frame_ = 0;
}
ExplosionObject ::~ExplosionObject()
{
}

bool ExplosionObject ::LoadImg(std::string path, SDL_Renderer *screen)
{
    bool ret = BaseObject ::LoadImg(path, screen);
    if (ret == true)
    {
        frame_width_ = rect_.w / NUM_FRAME_EXP;
        frame_height_ = rect_.h;
    }
    return ret;
}
void ExplosionObject ::set_clips()
{
    if (frame_width_ > 0 && frame_height_ > 0)
    {
        frame_clip_[0].x = 0;
        frame_clip_[0].y = 0;
        frame_clip_[0].w = frame_width_;
        frame_clip_[0].h = frame_height_;

        frame_clip_[1].x = frame_width_;
        frame_clip_[1].y = 0;
        frame_clip_[1].w = frame_width_;
        frame_clip_[1].h = frame_height_;

        frame_clip_[2].x = 2 * frame_width_;
        frame_clip_[2].y = 0;
        frame_clip_[2].w = frame_width_;
        frame_clip_[2].h = frame_height_;

        frame_clip_[3].x = 3 * frame_width_;
        frame_clip_[3].y = 0;
        frame_clip_[3].w = frame_width_;
        frame_clip_[3].h = frame_height_;

        frame_clip_[4].x = 4 * frame_width_;
        frame_clip_[4].y = 0;
        frame_clip_[4].w = frame_width_;
        frame_clip_[4].h = frame_height_;

        frame_clip_[5].x = 5 * frame_width_;
        frame_clip_[5].y = 0;
        frame_clip_[5].w = frame_width_;
        frame_clip_[5].h = frame_height_;

        frame_clip_[6].x = 6 * frame_width_;
        frame_clip_[6].y = 0;
        frame_clip_[6].w = frame_width_;
        frame_clip_[6].h = frame_height_;

        frame_clip_[7].x = 7 * frame_width_;
        frame_clip_[7].y = 0;
        frame_clip_[7].w = frame_width_;
        frame_clip_[7].h = frame_height_;
    }
}

void ExplosionObject::Show(SDL_Renderer *screen)
{
    SDL_Rect *current_clip = &frame_clip_[frame_];
    SDL_Rect render_quad = {rect_.x, rect_.y, frame_width_, frame_height_};
    if (current_clip != NULL)
    {
        render_quad.w = current_clip->w;
        render_quad.h = current_clip->h;
    }

    SDL_RenderCopy(screen, p_object_, current_clip, &render_quad);
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

class PlayerPower : public BaseObject
{
public:
    PlayerPower();
    ~PlayerPower();
    // hàm set vào số mạng
    void SetNum(const int &num) { number_ = num; }
    void AddPos(const int &xPos);
    void Show(SDL_Renderer *screen);
    void Init(SDL_Renderer *screen);
    // hàm tăng mạng
    void InitCrease();
    // hàm giảm mạng
    void Decrease();

private:
    // số mạng
    int number_;
    // các vị trí của ô hiển thị mạng của nhân vật
    std::vector<int> pos_list_;
};

class PlayerMoney : public BaseObject
{
public:
    PlayerMoney();
    ~PlayerMoney();

    void Init(SDL_Renderer *screen);
    void Show(SDL_Renderer *screen);
    void SetPos(const int &x, const int &y) { x_pos_ = x, y_pos_ = y; }

private:
    int x_pos_;
    int y_pos_;
};

PlayerPower ::PlayerPower()
{
    number_ = 0;
}
PlayerPower ::~PlayerPower()
{
}
void PlayerPower::AddPos(const int &xp)
{
    pos_list_.push_back(xp);
}

void PlayerPower ::Init(SDL_Renderer *screen)
{

    LoadImg("image//player_pw.png", screen);
    number_ = 3;
    // CÁI NÀY ĐỂ ĐẢM BẢO RẰNG DANH SÁCH VỊ TRÍ ĐANG TRỐNG ĐỂ CHO KHỎI TỐN TÀI NGUYÊN
    if (pos_list_.size() > 0)
    {
        pos_list_.clear();
    }
    AddPos(20);
    AddPos(60);
    AddPos(100);
}

void PlayerPower::Show(SDL_Renderer *screen)
{
    for (int i = 0; i < pos_list_.size(); i++)
    {
        rect_.x = pos_list_.at(i);
        rect_.y = 0;
        Render(screen);
    }
}

void PlayerPower::Decrease()
{
    number_--;
    pos_list_.pop_back();
}

void PlayerPower ::InitCrease()
{
    number_++;
    int last_pos = pos_list_.back();
    last_pos += 40;
    pos_list_.push_back(last_pos);
}

PlayerMoney ::PlayerMoney()
{
    x_pos_ = 0;
    y_pos_ = 0;
}

PlayerMoney ::~PlayerMoney()
{
}
void PlayerMoney ::Init(SDL_Renderer *screen)
{
    bool ret = LoadImg("image//money.png", screen);
}

void PlayerMoney ::Show(SDL_Renderer *screen)
{
    rect_.x = x_pos_;
    rect_.y = y_pos_;
    Render(screen);
}

typedef struct GeometricFormat
{
public:
    GeometricFormat(int left, int top, int width, int height) { left_ = left, top_ = top, width_ = width, height_ = height; };

    int left_;
    int top_;
    int width_;
    int height_;
};

typedef struct ColorData
{
    ColorData(Uint8 r, Uint8 g, Uint8 b) { red_ = r, green_ = g, blue_ = b; }

public:
    Uint8 red_;
    Uint8 green_;
    Uint8 blue_;
};

class Geometric
{
public:
    static void RenderRectangle(const GeometricFormat &geo_size, const ColorData &color_data, SDL_Renderer *screen);
    static void RenderOutline(const GeometricFormat &geo_size, const ColorData &color_data, SDL_Renderer *screen);
};

void Geometric ::RenderRectangle(const GeometricFormat &geo_size, const ColorData &color_data, SDL_Renderer *screen)
{
    SDL_Rect fill_rect = {geo_size.left_, geo_size.top_, geo_size.width_, geo_size.height_};
    SDL_SetRenderDrawColor(screen, color_data.red_, color_data.green_, color_data.blue_, 0xFF);
    SDL_RenderFillRect(screen, &fill_rect);
}

void Geometric ::RenderOutline(const GeometricFormat &geo_size, const ColorData &color_data, SDL_Renderer *screen)
{
    SDL_Rect outRect = {geo_size.left_, geo_size.top_, geo_size.width_, geo_size.height_};
    SDL_SetRenderDrawColor(screen, color_data.red_, color_data.green_, color_data.blue_, 0xFF);
    SDL_RenderDrawRect(screen, &outRect);
}

#define GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 10

#define PLAYER_SPEED 2
#define PLAYER_HIGHT_VAL 18;

#define FRAME_NUM_32 32

class BossObject : public BaseObject
{
public:
    BossObject();
    ~BossObject();
    void set_x_val(int xp) { x_val_ = xp; }
    void set_y_val(int yp) { y_val_ = yp; }

    void set_xpos(const int &xps) 
    { 
        x_pos_ = xps; 
        rect_.x = x_pos_ - map_x_;
    }
    void set_ypos(const int &yps) 
    {
         y_pos_ = yps;
         rect_.y = y_pos_ - map_y_;
     }
    int get_x_pos() { return x_pos_; }
    int get_y_pos() { return y_pos_; }

    void Show(SDL_Renderer *des);
    bool LoadImg(std::string path, SDL_Renderer *screen);
    void set_clips();

    int get_width_frame() const { return width_frame_; }
    int get_height_frame() const { return height_frame_; }

    void SetMapXY(const int map_x, const int map_y) { map_x_ = map_x, map_y_ = map_y; };

    void CheckToMap(Map &g_map);
    void CenterEntityOnMap(Map &g_map);
    void DoPlayer(Map &g_map);
    void InitPlayer();
    std::vector<BulletObject *> get_bullet_list() const { return bullet_list_; }
    void set_bullet_list(const std::vector<BulletObject *> &am_list) { bullet_list_ = am_list; }

    void InitBullet(SDL_Renderer *screen);
    void MakeBullet(SDL_Renderer *des, const int &x_limit, const int &y_limit);

    SDL_Rect GetRectFrame();
    void RemoveBullet(const int &idx);
    void setDie() { is_die = true; }
    bool getDie() { return is_die; }

private:
    int map_x_;
    int map_y_;
    int on_ground_;
    int think_time_;
    Input input_type_;
    int frame_;
    SDL_Rect frame_clip_[FRAME_NUM_32];
    int x_pos_;
    int y_pos_;
    float x_val_;
    float y_val_;
    int width_frame_;
    int height_frame_;
    bool is_die;
    std::vector<BulletObject *> bullet_list_;
};

BossObject::BossObject()
{
    frame_ = 0;
    x_val_ = 0.0;
    y_val_ = 0.0;
    x_pos_ = 0;
    y_pos_ = 0;
    width_frame_ = 0;
    height_frame_ = 0;
    think_time_ = 0;
    map_x_ = 0;
    map_y_ = 0;
}

BossObject::~BossObject()
{
}

bool BossObject::LoadImg(std::string path, SDL_Renderer *screen)
{
    bool ret = BaseObject::LoadImg(path, screen);

    if (ret == true)
    {
        width_frame_ = rect_.w / FRAME_NUM_32;
        height_frame_ = rect_.h;
    }

    return ret;
}

void BossObject::set_clips()
{
    // Clip the sprites
    if (width_frame_ > 0 && height_frame_ > 0)
    {
        for (int i = 0; i < FRAME_NUM_32; ++i)
        {
            frame_clip_[i].x = width_frame_ * i;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }
}

void BossObject::Show(SDL_Renderer *des)
{
    if (think_time_ == 0)
    {
        rect_.x = x_pos_ - map_x_;
        rect_.y = y_pos_ - map_y_;
        frame_++;
        if (frame_ >= 32)
        {
            frame_ = 0;
        }

        SDL_Rect *currentClip = &frame_clip_[frame_];
        SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame_, height_frame_};
        if (currentClip != NULL)
        {
            renderQuad.w = currentClip->w;
            renderQuad.h = currentClip->h;
        }

        SDL_RenderCopy(des, p_object_, currentClip, &renderQuad);
    }
}

SDL_Rect BossObject ::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;
    is_die = false;
    return rect;
}

void BossObject::RemoveBullet(const int &idx)
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

void BossObject::DoPlayer(Map &g_map)
{
    if (think_time_ == 0)
    {
        x_val_ = 0;
        y_val_ += GRAVITY_SPEED;

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
            if (on_ground_ == 1)
            {
                y_val_ = -PLAYER_HIGHT_VAL;
            }

            input_type_.jump_ = 0;
        }

        CheckToMap(g_map);

        // CenterEntityOnMap(g_map);
    }

    if (think_time_ > 0)
    {
        think_time_--;

        if (think_time_ == 0)
        {
            InitPlayer();
        }
    }
}

void BossObject::InitPlayer()
{
    x_val_ = 0;
    y_val_ = 0;

    if (x_pos_ > 256)
    {
        x_pos_ -= 256;
    }
    else
        x_pos_ = 0;
    y_pos_ = 0;
    think_time_ = 0;
    input_type_.left_ = 1;
}

void BossObject::CenterEntityOnMap(Map &g_map)
{
    g_map.start_x_ = x_pos_ - (SCREEN_WIDTH / 2);

    if (g_map.start_x_ < 0)
    {
        g_map.start_x_ = 0;
    }

    else if (g_map.start_x_ + SCREEN_WIDTH >= g_map.max_x_)
    {
        g_map.start_x_ = g_map.max_x_ - SCREEN_WIDTH;
    }

    g_map.start_y_ = y_pos_ - (SCREEN_HEIGHT / 2);

    if (g_map.start_y_ < 0)
    {
        g_map.start_y_ = 0;
    }

    else if (g_map.start_y_ + SCREEN_HEIGHT >= g_map.max_y_)
    {
        g_map.start_y_ = g_map.max_y_ - SCREEN_HEIGHT;
    }
}

void BossObject::CheckToMap(Map &g_map)
{
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;

    on_ground_ = 0;

    // Check Horizontal
    int height_min = height_frame_; // SDLCommonFunc::GetMin(height_frame_, TILE_SIZE);

    /*
    x1,y1***x2
    *       *
    *       *
    *       *
    *y2******

    */
    x1 = (x_pos_ + x_val_) / TILE_SIZE;
    x2 = (x_pos_ + x_val_ + width_frame_ - 1) / TILE_SIZE;

    y1 = (y_pos_) / TILE_SIZE;
    y2 = (y_pos_ + height_min - 1) / TILE_SIZE;

    // Check x1, x2 with full width of map
    // Check y1, y2 with full heigth of map
    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if (x_val_ > 0) // when object is moving by right  ===>
        {
            // Check current position of map. It is not blank_tile.
            if ((g_map.tile[y1][x2] != BLANK_TILE) || (g_map.tile[y2][x2] != BLANK_TILE))
            {
                // Fixed post of object at current post of map.
                // => Cannot moving when press button
                x_pos_ = x2 * TILE_SIZE;
                x_pos_ -= width_frame_ + 1;
                x_val_ = 0; // cannot moving
            }
        }
        else if (x_val_ < 0) // When moving by left    <====
        {
            if ((g_map.tile[y1][x1] != BLANK_TILE) || (g_map.tile[y2][x1] != BLANK_TILE))
            {
                x_pos_ = (x1 + 1) * TILE_SIZE;
                x_val_ = 0;
            }
        }
    }

    // Check vertical
    int width_min = width_frame_; // SDLCommonFunc::GetMin(width_frame_, TILE_SIZE);

    x1 = (x_pos_) / TILE_SIZE;
    x2 = (x_pos_ + width_min) / TILE_SIZE;

    y1 = (y_pos_ + y_val_) / TILE_SIZE;
    y2 = (y_pos_ + y_val_ + height_frame_) / TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if (y_val_ > 0)
        {
            // Similar for vertical
            if ((g_map.tile[y2][x1] != BLANK_TILE) || (g_map.tile[y2][x2] != BLANK_TILE))
            {
                y_pos_ = y2 * TILE_SIZE;
                y_pos_ -= height_frame_;

                y_val_ = 0;

                on_ground_ = 1;
            }
        }
        else if (y_val_ < 0)
        {
            if ((g_map.tile[y1][x1] != BLANK_TILE) || (g_map.tile[y1][x2] != BLANK_TILE))
            {
                y_pos_ = (y1 + 1) * TILE_SIZE;

                y_val_ = 0;
            }
        }
    }

    // If there is not collision with map tile.
    x_pos_ += x_val_;
    y_pos_ += y_val_;

    if (x_pos_ < 0)
    {
        x_pos_ = 0;
    }
    else if (x_pos_ + width_frame_ >= g_map.max_x_)
    {
        x_pos_ = g_map.max_x_ - width_frame_ - 1;
    }

    if (y_pos_ > g_map.max_y_)
    {
        think_time_ = 60;
    }
}

void BossObject::InitBullet(SDL_Renderer *screen)
{
    BulletObject *p_bullet = new BulletObject();
    p_bullet->set_bullet_type(BulletObject ::BOSS_BULLET);

    p_bullet->LoadImgBullet(screen);

    int xp = x_pos_ - map_x_;
    int yp = y_pos_ - map_y_;

    p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
    p_bullet->set_is_move(true);
    p_bullet->SetRect(xp - 50, yp + height_frame_ - 30);
    p_bullet->set_x_val(15);
    bullet_list_.push_back(p_bullet);
}

void BossObject::MakeBullet(SDL_Renderer *des, const int &x_limit, const int &y_limit)
{
    if (frame_ == 18)
    {
        InitBullet(des);
    }

    for (int i = 0; i < bullet_list_.size(); i++)
    {
        BulletObject *p_bullet = bullet_list_.at(i);
        if (p_bullet != NULL)
        {
            if (p_bullet->get_is_move())
            {
                int val = rect_.x + width_frame_ - p_bullet->GetRect().x;
                if(val <600 && val >0 )
                {
                    p_bullet->HandleMove(x_limit, y_limit);
                    p_bullet->Render(des);
                }
                else 
                {
                    p_bullet -> set_is_move(false);

                }
            }
            else
            {
                p_bullet->free();
                bullet_list_.erase(bullet_list_.begin() + i);
            }
        }
    }
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

        if (g_sound_bullet[0] == NULL || g_sound_bullet[1] == NULL || g_sound_explosion == NULL)
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
    bool ret = g_background.LoadImg("image//back2.png", g_screen);
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
                    xm = g_event.button.x;
                    ym = g_event.button.y;
                    for (int i =0; i<kMenuItemNum ; i++)
                    {
                        if(checkpositionforrect(xm , ym , text_menu[i].GetRech()))
                        {
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



int main(int argc, char *argv[])
{

    ImpTimer fps_timer;
    
    if (InitData() == false)
    {
        return -1;
    }

    // if(LoadMenu() == false)
    // {
    //     return -1;
    // }
    if (LoadBackground() == false)
    {
        return -1;
    }

    GameMap game_map;
    game_map.LoadMap("map//map.dat");
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

    bool tRet = exp_threat.LoadImg("image//ex2.png", g_screen);
    if (tRet == false)
    {
        return -1;
    }
    else
    {
        exp_threat.set_clips();
    }

    ExplosionObject exp_main;

    bool mRet = exp_main.LoadImg("image//ex2.png", g_screen);
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

    TextObject mark_game;
    mark_game.SetColor(TextObject ::BLACK_TEXT);
    UINT mark_value = 0;

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
                    if(  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"h","d",g_window) == 0 )
                    {
                        p_threat -> free();
                        close();
                        SDL_Quit();
                        return 0;

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
                    if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "h", "you die", g_window) == 0)
                    {
                        bossObject.free();
                        close();
                        SDL_Quit();
                        return 0;   
                    }
                    // quit  = true
                    // int check = gameover();
                    // if(check = again)
                    // {
                    //     quit = false;
                    //     khởi tạo toàn bộ
                    //     giống như bna đầu;

                    // }
                    // else{
                    //     quit = true;
                    // }
                    // đi vào showMenu
                    // play TRY_AGAIN false mạng vị trí nhân vật backbround mấy con nhân vật 
                    // play exit true
                }       
            }
        }

        // SHOW GAME TIME
        std::string str_time = "TIME: ";
       // Uint32 time_val = SDL_GetTicks() / 1000;
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
            // tạo và tải một hình ảnh chữ từ một chuỗi kí tự
            time_game.LoadFromRenderText(font_time, g_screen);
            time_game.RenderText(g_screen, SCREEN_WIDTH - 150, 15);
        }

        std::string val_str_mark = std::to_string(mark_value);
        std::string strMark("MARK: ");
        strMark += val_str_mark;

        mark_game.SetText(strMark);
        mark_game.LoadFromRenderText(font_time, g_screen);
        mark_game.RenderText(g_screen, SCREEN_WIDTH * 0.5-100, 15);
        
        int money_count = p_player.GetMoneyCount();
        std::string money_str = std::to_string(money_count);
        money_game.SetText(money_str);
        money_game.LoadFromRenderText(font_time, g_screen);
        money_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 300, 15);

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
    close();
    return 0;
}
