#include "playground.hpp"
#include <vector>

#define DEBUG


Snake::Snake(){
    snake.push_back(new Square(6,3,RIGHT));
    snake[0]->r = RSNAKE2;    
    snake[0]->g = GSNAKE2;    
    snake[0]->b = BSNAKE2;    
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
    switch(tail->dir){
        case TOP:
        snake.push_back(new Square(tail->posx,tail->posy+1,tail->dir));
        break;
        case BOTTOM:
        snake.push_back(new Square(tail->posx,tail->posy-1,tail->dir));
        break;
        case RIGHT:
        snake.push_back(new Square(tail->posx-1,tail->posy,tail->dir));
        break;
        case LEFT:
        snake.push_back(new Square(tail->posx+1,tail->posy,tail->dir));
        break;
    }
}
// if each pos = &pos-1 => direction would be necessary only for the head no?
// can't manage border properly like this because blocks go out of bounce
// would be preferable that each current block gets the position of the next
// from tail to head
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
// void Snake::updatePos(){
//     int bufPosx;
//     int bufPosy;
//     for(int i=snake.size()-1 ; i>0 ; i--){
//         // current = current-1 or current+1
//         // copy current for the next block!
//         bufPosx = snake[i]->posx;
//         bufPosy = snake[i]->posy;
//         snake[i]->posx = snake[i-1]->posx;
//         snake[i]->posy = snake[i-1]->posy;
//     }
// }
std::vector<Square*> Snake::getSnake(){
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
    for(int i=0 ; i<snake.size() ; i++){        
        std::cout << "Square " << i << ": posX: " << snake[i]->posx << " posY: " << 
        snake[i]->posy << " dir: " << snake[i]->dir << std::endl;
    }
    for(int i=0 ; i<dirs.size() ; i++){
        std::cout << "dir" << i << " = " << dirs[i] << std::endl;
    }
}