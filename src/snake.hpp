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
    
    public:
    Snake();
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