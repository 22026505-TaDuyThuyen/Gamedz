#ifndef PLAYER_POWER_H_
#define PLAYER_POWER_H_

#include "CommonFunc.h"
#include "BaseObject.h"

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

#endif
