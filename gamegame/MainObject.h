
#ifndef MAIN_OBJECT_H_
#define MAIN_OBJECT_H_

#include <vector>  
#include "CommonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"

#define GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 10
#define PLAYER_SPEED   10
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

    bool LoadImg(std::string path, SDL_Renderer *screen);
    // phai show theo tung frame
    void Show(SDL_Renderer *des);
    void HandelInputAction(SDL_Event events, SDL_Renderer *screen);
    void set_clips();

    // map_data là tham số của bản đồ
    void DoPlayer(Map &map_data);
    // kiem tra xem nhân vật đã va chạm vào map chưa để đứng được trên map
    void CheckToMap(Map &map_data);
    // hàm set vào 2 tham số của bản đồ

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

    void setpos(const int&x, const int& y) {x_pos_=x,y_pos_ = y; }

    int GetDecreaseMoney() const {return money_count -15;}
    
    ReSetMoneyCount() { money_count = 0; }

private:
    int money_count;

    std::vector<BulletObject *> p_bullet_list_;

    float x_val_;
    float y_val_;

    float x_pos_;
    float y_pos_;

    int width_frame_;
    int height_frame_;

    SDL_Rect frame_clip_[8];
    // bien ve trang thai
    Input input_type_;
    int frame_;
    // biến lưu trạng thái của hai kiểu di chuyển là sang phải hay sang trái;
    int status_;
    bool on_ground_;
    int map_x_;
    int map_y_;
    int come_back_time_;
};


 #endif