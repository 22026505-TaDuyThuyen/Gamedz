// #ifndef BASE_OBJECT_H_
// #define BASE_OBJECT_H_

#include "CommonFunc.h"

class BaseObject
{
public:
    BaseObject();
    ~BaseObject();
    void SetRect(const int& x,const int& y){rect_.x=x,rect_.y=y;}//ham set kich thuoc cho rect
    SDL_Rect GetRect() const {return rect_;}
    SDL_Texture*GetObject() const {return p_object_;} //ham lay ra p_object_

    //ham load anh bitmap len tren doi tuong p_object_ sau do dung p_object_ dua len man hinh
    virtual bool LoadImg(std::string path,SDL_Renderer*screen);
    void Render(SDL_Renderer*des,const SDL_Rect*clip=NULL); 
    void free();
protected:
    SDL_Texture*p_object_;//luu tru hinh anh;
    SDL_Rect rect_;//luu kich thuoc;

};


// #endif