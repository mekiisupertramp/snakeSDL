//
//  playground.hpp
//
//  Created by Mehmed Blazevic on 08.05.2025.
//

#ifndef playground_hpp
#define playground_hpp

#include <iostream>
#include <unistd.h>
#include <math.h>
#include <vector>
#include <random>
#include <SDL2/SDL.h>

#define RSNAKE  100
#define BSNAKE  100
#define GSNAKE  0

#define RSNAKE2  0
#define BSNAKE2  100
#define GSNAKE2  100

#define RPLAY   10
#define BPLAY   10
#define GPLAY   10

#define RTARG   100
#define BTARG   0
#define GTARG   100

#define LC  255 // lines color

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
    Square(int x, int y, int r, int g, int b);
    Square(int x, int y, Direction di);
};

class Snake{
    private:
    std::vector<Square*> snake;
    std::vector<Direction> dirs;
    
    public:
    Snake();
    ~Snake();    
    void addBlock();
    void updateDir();
    void updatePos();
    void setDir(Direction dir);
    std::vector<Square*> getSnake();
    Square* getHead();
    void debug();
};

enum Collision{
    NONE, 
    WALL,
    SNAKE,
    TARGET
};

class Playground{
    private: 
    SDL_Renderer* ren;
    int width;
    int height;
    int rectWidth;    
    int rectHeight;   
    int dif; 
    std::vector<Square*> pixels;
    
    void bordersManagement(Snake* s, int difficulty);
    void border0(Snake* s);
    void border1(Snake* s);
    void border2(Snake* s);
    void border3(Snake* s);
    

    public:
    Playground(SDL_Window* window, SDL_Renderer* renderer, int width, int height, int difficulty);
    ~Playground();

    void update(Snake* snake);    
    void update(Snake* snake, Square* target);
    void update(Snake* snake, Square* target, int difficulty);
    void render();
    Collision getCollision(Snake* snake, Square* target);
    bool isTargetPosOK(Snake* snake, Square* target);
};


#endif /* playground_hpp */