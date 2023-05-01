//LUU Y VE DAU PHAY O DONG 18
//luu y them tu khoa virtual o baseobject.h
// #ifndef MAIN_OBJECT_H_
// #define MAIN_OBJECT_H_

#include <vector>  
#include "CommonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"

//4.1
//TỐC ĐỘ RƠI
#define GRAVITY_SPEED 0.8
//TỐC ĐỘ RƠI TỰ DO TỐI ĐA
#define MAX_FALL_SPEED 10
// TỐC ĐỘ DI CHUYỂN NHÂN VẬT
#define PLAYER_SPEED 40

#define PLAYER_JUMP_VAL 20

class MainObject :public BaseObject
{

public:
    MainObject();
    ~MainObject();

    enum WalkType
    {
        WALK_NONE =0,
        WALK_RIGHT=1,
        WALK_LEFT=2,
    };

//can co thong so rieng cho kich thuoc tam anh
    bool LoadImg(std::string path ,SDL_Renderer*screen);
 //phai show theo tung frame   
    void Show(SDL_Renderer*des);
    //ham xu li su kien khi bam trai phai di chuyen 
    void HandelInputAction(SDL_Event events,SDL_Renderer*screen);
    //hàm chuyên sử lí các tạo hiệu ứng clip
    void set_clips();
    //hàm cộng trừ một lượng cho nhân vật di chuyển can truyen vao ban do de nhan vat khong di qua cho phep
    //4.1
    //map_data là tham số của bản đồ
    void DoPlayer(Map&map_data);
    //kiem tra xem nhân vật đã va chạm vào map chưa để đứng được trên map
    void CheckToMap(Map&map_data);
    //hàm set vào 2 tham số của bản đồ
    //4.2
    void SetMapXY(const int  map_x,const int map_y){map_x_=map_x;map_y_=map_y;}
    //hàm tính toán thông số của map khi di chuyển nhân vật
    void CenterEntityOnMap(Map& map_data);
    // hàm chuyên cho update các img khi nhảy một img khi chạy một img
    void UpdateImagePlayer(SDL_Renderer*des);


    SDL_Rect GetRectFrame();

    void set_bullet_list(std::vector <BulletObject*> bullet_list)
    {
        p_bullet_list_ = bullet_list;
    }
    std:: vector<BulletObject*> get_bullet_list() const {return p_bullet_list_;}
    void HandleBullet(SDL_Renderer*des);
    void RemoveBullet(const int& idx);
    void IncreaseMoney();

    int GetMoneyCount () const {return money_count;}

    void set_comeback_time(const int& cb_time) { come_back_time_ = cb_time; }

    int get_frame_width() const {return width_frame_;}

    int get_frame_height() const {return height_frame_;}
private:
    int money_count ;
//tạo 1 vector để lưu trữ băng đạn
    std::vector<BulletObject*> p_bullet_list_;
    
    
    //bien tang len bao nhieu
    float x_val_;
    float y_val_;

//vi tri nhan vat de show
    float x_pos_;
    float y_pos_;

//kich thuoc tung frame nhan vat
    int width_frame_;
    int height_frame_; 

//luu cac frame: co 8 frame
    SDL_Rect frame_clip_[8];
    //bien ve trang thai
    Input input_type_;
    //bien luu chi so frame
    int frame_;
    //biến lưu trạng thái của hai kiểu di chuyển là sang phải hay sang trái;
    int status_;
    //trạng thái khi nhân vật đứng trên mặt đất r
    bool on_ground_;

    //4.2 thêm 2 biến map_x và map_y để lưu vị trí của bản đồ
    int map_x_;
    int map_y_;

    // biến mới bài 7 
    //thời gian chờ đợi xuất hiện trở lại
    int come_back_time_;
};


// #endif