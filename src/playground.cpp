#include "playground.hpp"
#include <vector>

#define DEBUG

Playground::Playground(int width, int height, int difficulty):width(width),height(height),dif(difficulty){
    std::srand(std::time(nullptr)); 
    for(int y=0 ; y<height ; y++){
        for(int x=0 ; x<width ; x++){
            pixels.push_back(new Square(x,y,RPLAY,GPLAY,BPLAY));
        }
    } 
}
Playground::~Playground(){
    for(auto p : pixels){
        delete p;
    }
}
const std::vector<Square*>& Playground::getPixels(){return pixels;}
int Playground::getWidth(){return width;}
int Playground::getHeigth(){return height;}
int Playground::getDifficulty(){return dif;}

Collision Playground::getCollision(Snake* snake, Square* target){
    Square* head = snake->getHead();
    std::vector<Square*> s = snake->getSnake();
    if((head->posx==target->posx)&&(head->posy==target->posy)) return TARGET;

    for(int i=1 ; (size_t)i<snake->getSnake().size() ; i++){   
        if((head->posx==s[i]->posx)&&(head->posy==s[i]->posy)) return SNAKE;   
    } 
    if((head->posx<0)||(head->posx>width-1)|| (head->posy<0)||(head->posy>height-1)) {
        return WALL; 
    }
    
    return NONE;
}
bool Playground::isTargetPosOK(Snake* snake, Square* target){
    std::vector<Square*> sn = snake->getSnake();
    for(auto s : sn){
        if((s->posx==target->posx)&&(s->posy==target->posy)) return false;
    }
    return true;
}

void Playground::border0(Snake* snake){
    for(auto s : snake->getSnake()){
        if((s->posx<0)||(s->posx>width)) s->activ = false;
        if((s->posy<0)||(s->posy>height)) s->activ = false;
    }
}
void Playground::border1(Snake* snake){
    for(auto s : snake->getSnake()){
        s->posx = (s->posx + width) % width;
        s->posy = (s->posy + height) % height;
    }   
}

void Playground::border2(Snake* snake){
    for(auto b : snake->getSnake()){
        if(b->posx<0){
            if(b->posy==height-1){
                b->posx = width-1;
                b->posy = 0;
                b->dir = LEFT;
            }else{
                if(b->posy==0){
                    b->posx = width-1;
                    b->posy = height-1;
                    b->dir = LEFT;
                }else{
                    b->posx = b->posy;
                    b->posy = 0;
                    b->dir = BOTTOM;
                }
            }
        }
        if(b->posx>width-1){
            b->posx = b->posy;
            b->posy = height-1;
            b->dir = TOP;
        }
        if(b->posy>height-1){
            b->posy = b->posx;
            b->posx = width-1;            
            b->dir = LEFT;
        }
        if(b->posy<0){
            b->posy = b->posx;
            b->posx = 0;            
            b->dir = RIGHT;
        }
    }   
}
// bug somwhere because [1] get same position as [0]
void Playground::border3(Snake* snake){
    Square* head = snake->getHead();  
    //   0 → left edge, 1 → bottom edge,
    //   2 → right edge, 3 → top edge  
    int edge = std::rand() % 4;
    // random snake's head position
    // get a new position by avoiding previous one
    if((head->posx<0)||(head->posx>width-1)||(head->posy<0)||(head->posy>height-1)){
        switch (edge){
            case 0: head->posx = 0; head->dir = RIGHT; do{edge = std::rand()%height;}while(edge == head->posy); head->posy=edge; break;
            case 1: head->posy = height-1; head->dir = TOP; do{edge = std::rand() % width;}while(edge == head->posx); head->posx=edge; break;
            case 2: head->posx = width-1; head->dir = LEFT; do{edge = std::rand()%height;}while(edge == head->posy); head->posy=edge; break;
            case 3: head->posy = 0; head->dir = BOTTOM; do{edge = std::rand()%width;}while(edge ==head->posx); head->posx=edge; break;            
            default: break;
        }
    }

    // distribute position
    for(int i=1 ; (size_t)i<snake->getSnake().size() ; i++){
        if((snake->getSnake()[i]->posx<0)||(snake->getSnake()[i]->posx>width-1)||(snake->getSnake()[i]->posy<0)||(snake->getSnake()[i]->posy>height-1)){
            snake->getSnake()[i]->posx = snake->getSnake()[i-1]->posx;
            snake->getSnake()[i]->posy = snake->getSnake()[i-1]->posy;
            snake->getSnake()[i]->dir = snake->getSnake()[i-1]->dir;

            // needed because snake's position is updated before borders are assess
            switch(snake->getSnake()[i]->dir){
                case RIGHT: snake->getSnake()[i]->posx -=1; break;
                case LEFT: snake->getSnake()[i]->posx +=1; break;
                case TOP: snake->getSnake()[i]->posy +=1; break;
                case BOTTOM: snake->getSnake()[i]->posy -=1; break;
            }
        }
    }
}
void Playground::bordersManagement(Snake* s, int difficulty){
    switch(difficulty){
        case 0: border0(s); break;
        case 1: border1(s); break;
        case 2: border2(s); break;
        case 3: border3(s); break; 
    }   
}

void Playground::update(Snake* snake, Square* target, int difficulty){
    dif = difficulty;
    update(snake,target);
}
void Playground::update(Snake* snake){  
    snake->updatePos();  
    snake->updateDir();
    bordersManagement(snake,dif); // need difficulty level from main
#ifdef DEBUG
    snake->debug();
#endif
    for(auto p: pixels){
        p->activ=false;
    }
    for(auto s: snake->getSnake()){
        if(s->posx<0) break; // prevent bad access to pixels
        if(s->posy<0) break;
        if(s->posx>width-1) break;
        if(s->posy>height-1) break;
        pixels[s->posx+s->posy*width]->activ = s->activ;
        pixels[s->posx+s->posy*width]->r = s->r;
        pixels[s->posx+s->posy*width]->g = s->g;
        pixels[s->posx+s->posy*width]->b = s->b;        
    }
     
}

void Playground::update(Snake* snake, Square* target){  
    update(snake);
    for(auto p: pixels){
        if((p->posx==target->posx)&&(p->posy==target->posy)){
            p->activ=true;
            p->r = target->r;
            p->g = target->g;
            p->b = target->b;
        }
    }
}