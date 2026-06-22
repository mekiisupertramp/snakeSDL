#include "playground.hpp"
#include <vector>

#define DEBUG


Snake::Snake():bodyR(RSNAKE),bodyG(GSNAKE),bodyB(BSNAKE){
    snake.push_back(new Square(6,3,RIGHT));
    snake[0]->r = RSNAKE2;
    snake[0]->g = GSNAKE2;
    snake[0]->b = BSNAKE2;
}

Snake::Snake(int x, int y, Direction dir, int headR, int headG, int headB, int bodyR, int bodyG, int bodyB):bodyR(bodyR),bodyG(bodyG),bodyB(bodyB){
    snake.push_back(new Square(x, y, dir));
    snake[0]->r = headR;
    snake[0]->g = headG;
    snake[0]->b = headB;
}

Snake::~Snake(){
    dirs.clear();
    for(auto s: snake){
        delete s;
    }
}

Square* Snake::getHead(){return snake[0];}
void Snake::addBlock(){  
    Square* tail = snake[snake.size()-1];
    Square* block = nullptr;
    switch(tail->dir){
        case TOP:
        block = new Square(tail->posx,tail->posy+1,tail->dir);
        break;
        case BOTTOM:
        block = new Square(tail->posx,tail->posy-1,tail->dir);
        break;
        case RIGHT:
        block = new Square(tail->posx-1,tail->posy,tail->dir);
        break;
        case LEFT:
        block = new Square(tail->posx+1,tail->posy,tail->dir);
        break;
    }
    block->r = bodyR;
    block->g = bodyG;
    block->b = bodyB;
    snake.push_back(block);
}

void Snake::updatePos(){   
    for(auto s : snake){
        switch(s->dir){
            case TOP:    s->posy = s->posy-1; break;
            case BOTTOM: s->posy = s->posy+1; break;
            case RIGHT:  s->posx = s->posx+1; break;
            case LEFT:   s->posx = s->posx-1; break;
        }        
    }
}
const std::vector<Square*>& Snake::getSnake(){
    return snake;
}
void Snake::setDir(Direction dir){
    if(dirs.size()<5){
        dirs.push_back(dir);
    }
}
void Snake::updateDir(){
    //first update the direction of each snake's block except the head
    for(int i=snake.size()-1 ; i>0 ; i--){
        snake[i]->dir = snake[i-1]->dir;
    }     

    if(dirs.empty()) return;
    
    // get values in dirs[], and skip the wrong ones
    bool found = false;
    Square* head = snake[0];   
    Direction dir;    

    while(!found){
        dir = dirs.front();
        dirs.erase(dirs.begin());
        switch(head->dir){
            case TOP:    if((dir==LEFT)||(dir==RIGHT)) head->dir = dir; break;                   
            case BOTTOM: if((dir==LEFT)||(dir==RIGHT)) head->dir = dir; break;
            case RIGHT:  if((dir==TOP)||(dir==BOTTOM)) head->dir = dir; break;
            case LEFT:   if((dir==TOP)||(dir==BOTTOM)) head->dir = dir; break;
        }     
        found = true;
    }
}
void Snake::debug(){
    std::cout << "--------- snake's Square ---------" << std::endl;
    for(int i=0 ; (size_t)i<snake.size() ; i++){        
        std::cout << "Square " << i << ": posX: " << snake[i]->posx << " posY: " << 
        snake[i]->posy << " dir: " << snake[i]->dir << std::endl;
    }
    for(int i=0 ; (size_t)i<dirs.size() ; i++){
        std::cout << "dir" << i << " = " << dirs[i] << std::endl;
    }
}
