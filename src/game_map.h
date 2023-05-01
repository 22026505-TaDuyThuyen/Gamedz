// #ifndef GAME_MAP_H
// #define GAME_MAP_H


//them 4.2

#include "CommonFunc.h"
#include "BaseObject.h"
//phai xay dung doi tuong du lieu hinh anh
#define MAX_TILES 20

class TileMat : public BaseObject//hinh anh tinh duoc load vao cac o, ke thua tu baseobject
{
    public:
    TileMat() {;}
    ~TileMat() {;}
    
}; 
//xay dung mot lop ket hop voi tilemat fill toan bo anh vao vi tri o vuong(vi tri luuu trong struct o comm.h)
class GameMap
{
public:
    GameMap() {;}
    ~GameMap() {;}
//ham load file so do map 
    void LoadMap(char*name);
//ham load toan bo hinh anh cho tiles map
    void LoadTiles(SDL_Renderer*screen);
//ham xay dung map fill toan bo hinh anh vao cac vi tri 
    void DrawMap(SDL_Renderer*screen);
    
    Map getMap() const {return game_map_;}; //4.1
    void SetMap(Map& map_data) { game_map_=map_data;};
private:
    Map game_map_; //chua toan bo thong tin ve ban do gom trang thai o vi tri o chi so o 
//doi tuong luu tru hinh anh (loai hinh ảnh ví dụ như đá, đạn , ... )
    TileMat tile_mat[MAX_TILES];
    
};

// #endif