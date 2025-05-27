//
//  square.hpp
//

#ifndef square_hpp
#define square_hpp

#define RSNAKE  100
#define BSNAKE  100
#define GSNAKE  0

#define RPLAY   10
#define BPLAY   10
#define GPLAY   10

enum Direction{
    TOP, 
    BOTTOM,
    LEFT,
    RIGHT
};

class Square{
    public:
    int posx;
    int posy;
    bool activ;
    int r;
    int g;
    int b;
    Direction dir;

    Square();
    ~Square() = default;
    Square(int x, int y);
    Square(int x, int y, Direction di);
    Square(int x, int y, int r, int g, int b);
};

#endif /* square_hpp */