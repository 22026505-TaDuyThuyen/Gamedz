// #ifndef THREATS_OBJECT_H_
// #define THREATS_OBJECT_H_

#include "CommonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"


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
        STATIC_THREAT =0,
        // trong một phạm vi
        MOVE_IN_SPACE_THREAT =1,
    };

// giá trị set của x_val_ và y thì chỉ cần truyền vào chứ không cần lấy ra
    void set_x_val(const float& xVal) { x_val_ = xVal ; }
    void set_y_val(const float& yVal) { y_val_ = yVal ; }

    void set_x_pos(const float& xp)  { x_pos_ = xp; }
    void set_y_pos(const float& yp)  { y_pos_ = yp; }
    float get_x_pos() const { return x_pos_; }
    float get_y_pos() const { return y_pos_; }
    void SetMapXY( const int& mp_x, const int& mp_y ) { map_x_=mp_x ; map_y_ = mp_y; }

    // hàm truyền sử lí tạo các hiệu ứng clip
    void set_clips();
    bool LoadImg(std :: string path, SDL_Renderer*screen  );
    void Show(SDL_Renderer*des);
    // hai hàm get sau đây sẽ cần trong bài toán check va chạm
    int get_width_frame() const { return width_frame_ ;}
    int get_height_frame() const { return height_frame_; }

// hàm xử lí nhân vật di chuyển hay đứng yên hay va chạm với bản đồ
    void DoPlayer(Map& gMap);
    void CheckToMap(Map& gMap);
// ở đây không cần centerentityomap vì map chỉ kéo theo khi ta cần di chuyển player thôi

// hàm khởi tạo ban đầu cho quái sau khi rơi xuống vực
    void InitReturn();
    void set_type_move(const int& typeMove) { type_move_ = typeMove; }
    // hàm xét vị trí để di chuyển 
    void SetAnimationPos(const int& pos_a, const int& pos_b) { animation_a_= pos_a, animation_b_= pos_b; }
    // ham dat trang thai cho quai
    void set_input_left(const int& ipLeft) {input_type_.left_ = ipLeft; }
    // ham xu li di chuyen trong pham vi a b
    void ImpMoveType(SDL_Renderer*screen);
    
    SDL_Rect GetRectFrame();
    std::vector <BulletObject*> get_bullet_list() const { return bullet_list_; }
    void set_bullet_list(const std:: vector<BulletObject*>& bl_list) { bullet_list_=bl_list; }

    void InitBullet (BulletObject*p_bullet,SDL_Renderer*screen);
    void MakeBullet(SDL_Renderer*screen,const int& x_limit,const int& y_limit);

    void RemoveBullet(const int& idx);
private:

// 2 biến xác định giới hạn của quái với bản đồ để khi rơi xuống vực thẳm còn check
    int map_x_;
    int map_y_;

//2 biến tăng giá trị
    float x_val_;
    float y_val_;
// vị trí hiện tại là vị trí bao nhiêu
    float x_pos_;
    float y_pos_;
//nằm trên mặt đất hay chưa
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
    //giới hạn a,b trong đó a là bên tay trái, b là bên tay phải ; quái di chuyển trong phạm vi từ a đến b
    int animation_a_;
    int animation_b_;
    // biến quản lí việc di chuyển 
    Input input_type_;

    std :: vector<BulletObject*> bullet_list_;



};


// #endif