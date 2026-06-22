//
//  playground.hpp
//

#ifndef snake_hpp
#define snake_hpp

#include "playground.hpp"
#include "square.hpp"

class Snake{
    private:
    std::vector<Square*> snake;
    std::vector<Direction> dirs;
    int bodyR;
    int bodyG;
    int bodyB;
    
    public:
    Snake();
    Snake(int x, int y, Direction dir, int headR, int headG, int headB, int bodyR, int bodyG, int bodyB);
    ~Snake();    
    void addBlock();
    void updateDir();
    void updatePos();
    void setDir(Direction dir);
    const std::vector<Square*>& getSnake();
    Square* getHead();
    void debug();
};

#endif /* snake_hpp */
