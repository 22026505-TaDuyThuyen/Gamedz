#include "BaseObject.h"

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
    SDL_Rect renderquad = {rect_.x, rect_.y, rect_.w, rect_.h}; 
    SDL_RenderCopy(des, p_object_, clip, &renderquad);         
}

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