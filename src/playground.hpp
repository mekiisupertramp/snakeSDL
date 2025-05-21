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

// delete Block class
// add direction here
// change Pixel into Square
class Pixel{
    public:
    int posx;
    int posy;
    bool activ;
    int r;
    int g;
    int b;

    Pixel();
    Pixel(int x, int y);
    Pixel(int x, int y, int active);
    Pixel(int x, int y, int active, int r, int g, int b);
};

enum Direction{
    TOP, 
    BOTTOM,
    LEFT,
    RIGHT
};
class Block: public Pixel{
    private:    
    public:
    Direction dir;

    Block();
    Block(int x, int y);
    Block(int x, int y, Direction di);
};

class Snake{
    private:
    std::vector<Block*> snake;
    std::vector<Direction> dirs;
    void insertPosition(int x, int y, Direction d);    
    void setPos(int x, int y);
    
    public:
    Snake();    
    void addBlock();
    void updateDir();
    void updatePos();
    void setDir(Direction dir);
    std::vector<Block*> getSnake();
    Block* getHead();
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
    int width;
    int height;
    int rectWidth;    
    int rectHeight;   
    int dif; 
    std::vector<Pixel*> pixels;
    SDL_Rect* rects;    
    
    void bordersManagement(Snake* s, int difficulty);
    //Collision getCollision(Snake* snake, Block* target);
    void border0(Snake* s);
    void border1(Snake* s);
    void border2(Snake* s);
    void border3(Snake* s);

    SDL_Window* win;
    SDL_Renderer* ren;

    public:
    Playground(SDL_Window* window, SDL_Renderer* renderer, int width, int height, int difficulty);
    Playground(int pixX, int pixY, int width, int height);

    void update(Snake* snake);    
    void update(Snake* snake, Block* target);
    void update(Snake* snake, Block* target, int difficulty);
    void render();
    Collision getCollision(Snake* snake, Block* target);
    bool isTargetPosOK(Snake* snake, Block* target);
};


#endif /* playground_hpp */