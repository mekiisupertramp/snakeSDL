#include "playground.hpp"
#include <vector>

#define DEBUG

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
Playground::~Playground(){
    for(auto p : pixels){
        delete p;
    }
}
Playground::Playground(SDL_Window* window, SDL_Renderer* renderer, int width, int height, int difficulty):ren(renderer),width(width),height(height),dif(difficulty){
    std::srand(std::time(nullptr));
    int w,h;
    SDL_GetWindowSize(window,&w, &h);
    rectWidth = w/width;
    rectHeight = h/height;   
    for(int y=0 ; y<height ; y++){
        for(int x=0 ; x<width ; x++){
            pixels.push_back(new Square(x,y,RPLAY,GPLAY,BPLAY));
        }
    } 
}

void Playground::render(){
    SDL_Rect rect = {.x=0, .y=0, .w=rectWidth, .h=rectHeight};
    for(auto p : pixels){
        rect.x = p->posx*rectWidth;
        rect.y = p->posy*rectHeight;
        if(p->activ){            
            SDL_SetRenderDrawColor(ren, p->r, p->g, p->b, 255); 
            SDL_RenderFillRect(ren,&rect);
            SDL_SetRenderDrawColor(ren, LC, LC, LC, 255);
            SDL_RenderDrawRect(ren,&rect);  
        }else{
            SDL_SetRenderDrawColor(ren, RPLAY, GPLAY, BPLAY, 255); 
            SDL_RenderFillRect(ren,&rect);
            SDL_SetRenderDrawColor(ren, LC, LC, LC, 255);
            SDL_RenderDrawRect(ren,&rect);  
        }
    }
    if(dif == 0){
        rect.x=0; rect.y=0;
        rect.w = width*rectWidth;
        rect.h = height*rectHeight;
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255); 
        SDL_RenderDrawRect(ren,&rect);
        rect.x=1; rect.y=1;
        rect.w = (width*rectWidth)-2;
        rect.h = (height*rectHeight)-2;
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255); 
        SDL_RenderDrawRect(ren,&rect);
    }
    SDL_RenderPresent(ren);
}

Collision Playground::getCollision(Snake* snake, Square* target){
    Square* head = snake->getHead();
    std::vector<Square*> s = snake->getSnake();
    if((head->posx==target->posx)&&(head->posy==target->posy)) return TARGET;

    for(int i=1 ; i<snake->getSnake().size() ; i++){   
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

void Playground::border0(Snake* s){
    for(auto s : s->getSnake()){
        if((s->posx<0)&&(s->posx>width)) s->activ = false;
        if((s->posy<0)&&(s->posy>height)) s->activ = false;
    }
}
void Playground::border1(Snake* s){
    for(auto s : s->getSnake()){
        s->posx = (s->posx<0) ? width-1 : s->posx;
        s->posx = (s->posx>width-1) ? 0: s->posx;
        s->posy = (s->posy<0) ? height-1 : s->posy;
        s->posy = (s->posy>height-1) ? 0: s->posy;
    }   
}

void Playground::border2(Snake* s){
    for(auto b : s->getSnake()){
        if((b->posx<0)&&(b->posy==0)){
            b->posx = width-1;
            b->posy = height-1;
            b->dir = LEFT;
        }
        if((b->posx<0)&&(b->posy==height-1)){
            b->posx = width-1;
            b->posy = 0;
            b->dir = LEFT;
        }

        if(b->posx<0){
            b->posx = b->posy;
            b->posy = 0;
            b->dir = BOTTOM;
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
void Playground::border3(Snake* s){
    Square* head = s->getHead();    
    int w = std::rand() % 4;
    // random snake's head position
    // get a new position and avoid previous one
    if((head->posx<0)||(head->posx>width-1)||(head->posy<0)||(head->posy>height-1)){
        switch (w){
            case 0: head->posx = 0; head->dir = RIGHT; do{w = std::rand()%height;}while(w == head->posy); head->posy=w; break;
            case 1: head->posy = height-1; head->dir = TOP; do{w = std::rand() % width;}while(w == head->posx); head->posx=w; break;
            case 2: head->posx = width-1; head->dir = LEFT; do{w = std::rand()%height;}while(w == head->posy); head->posy=w; break;
            case 3: head->posy = 0; head->dir = BOTTOM; do{w = std::rand()%width;}while(w ==head->posx); head->posx=w; break;            
            default: break;
        }
    }

    // distribute position from head
    for(int i=1 ; i<s->getSnake().size() ; i++){
        if((s->getSnake()[i]->posx<0)||(s->getSnake()[i]->posx>width-1)||(s->getSnake()[i]->posy<0)||(s->getSnake()[i]->posy>height-1)){
            s->getSnake()[i]->posx = s->getSnake()[i-1]->posx;
            s->getSnake()[i]->posy = s->getSnake()[i-1]->posy;
            s->getSnake()[i]->dir = s->getSnake()[i-1]->dir;

            // needed because snake's position is updated before borders are assess
            switch(s->getSnake()[i]->dir){
                case RIGHT: s->getSnake()[i]->posx -=1; break;
                case LEFT: s->getSnake()[i]->posx +=1; break;
                case TOP: s->getSnake()[i]->posy +=1; break;
                case BOTTOM: s->getSnake()[i]->posy -=1; break;
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
    snake->updatePos();  
    snake->updateDir();
    bordersManagement(snake,dif);
#ifdef DEBUG
    snake->debug();
#endif
    for(auto p: pixels){
        if((p->posx==target->posx)&&(p->posy==target->posy)){
            p->activ=true;
            p->r = target->r;
            p->g = target->g;
            p->b = target->b;
        }else{
            p->activ = false;
        }
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