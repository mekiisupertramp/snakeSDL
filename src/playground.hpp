//
//  playground.hpp
//

#ifndef playground_hpp
#define playground_hpp

#include <iostream>
#include <unistd.h>
#include <math.h>
#include <vector>
#include <random>
#include <ctime>
#include "snake.hpp"
#include "square.hpp"

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
    int dif; 
    std::vector<Square*> pixels;
    
    void bordersManagement(Snake* s, int difficulty);
    void border0(Snake* snake);
    void border1(Snake* snake);
    void border2(Snake* snake);
    void border3(Snake* snake);
    

    public:
    Playground(/*int wPxs, int hPxs, */int width, int height, int difficulty);
    ~Playground();

    void update(Snake* snake);    
    void update(Snake* snake, Square* target);
    void update(Snake* snake, Square* target, int box);
    Collision getCollision(Snake* snake, Square* target);
    bool isTargetPosOK(Snake* snake, Square* target);
    std::vector<Square*> getPixels();
    int getWidth();
    int getHeigth();
    int getDifficulty();
};


#endif /* playground_hpp */