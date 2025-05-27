#include "square.hpp"

Square::Square(): posx(0), posy(0), activ(0), r(RPLAY), g(BPLAY), b(GPLAY){}

Square::Square(int x, int y) : Square(){
    posx = x;
    posy = y;    
}
Square::Square(int x, int y, Direction di) : Square(x,y){
    activ=1;
    r=RSNAKE; 
    b=BSNAKE; 
    g=GSNAKE;
    dir=di;    
}
Square::Square(int x, int y, int r, int g, int b) : Square(x,y){
    activ=0;
    this->r=r;
    this->g=g;
    this->b=b;
}