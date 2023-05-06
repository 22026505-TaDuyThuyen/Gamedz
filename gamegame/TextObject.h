#ifndef TEXT_OBJECT_H_
#define TEXT_OBJECT_H_

#include "CommonFunc.h"
#include "BaseObject.h"

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
    bool LoadFromRenderText(TTF_Font *font, SDL_Renderer *screen);
    void Free();

    void SetColor(Uint8 red, Uint8 green, Uint8 blue);

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
    int GetHeight() const { return height_; }

    void SetText(const std::string &text) { str_val_ = text; }
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

#endif


