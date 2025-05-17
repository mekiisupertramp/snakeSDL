#include "playground.hpp"
#include <vector>

//#define DEBUG

Pixel::Pixel(): posx(0), posy(0), activ(0), r(0), g(0), b(0){}
Pixel::Pixel(int x, int y) : posx(x), posy(y){}
Pixel::Pixel(int x, int y, int active): posx(x), posy(y), activ(active){}
Pixel::Pixel(int x, int y, int active, int r, int g, int b): posx(x), posy(y), activ(active), r(r), g(g), b(b){}

Block::Block() : Pixel(){r=RSNAKE; b=BSNAKE; g=GSNAKE;}
Block::Block(int x, int y) : Pixel(x,y,true){r=RSNAKE; b=BSNAKE; g=GSNAKE;dir=RIGHT;}
Block::Block(int x, int y, Direction di) : Pixel(x,y,true){r=RSNAKE; b=BSNAKE; g=GSNAKE;dir=di;}

Snake::Snake(){
    snake.push_back(new Block(6,3,RIGHT));
    snake[0]->r = RSNAKE2;    
    snake[0]->g = GSNAKE2;    
    snake[0]->b = BSNAKE2;    
}
Block* Snake::getHead(){return snake[0];}
void Snake::addBlock(){  
    Block* tail = snake[snake.size()-1];
    switch(tail->dir){
        case TOP:
        snake.push_back(new Block(tail->posx,tail->posy+1,tail->dir));
        break;
        case BOTTOM:
        snake.push_back(new Block(tail->posx,tail->posy-1,tail->dir));
        break;
        case RIGHT:
        snake.push_back(new Block(tail->posx-1,tail->posy,tail->dir));
        break;
        case LEFT:
        snake.push_back(new Block(tail->posx+1,tail->posy,tail->dir));
        break;
    }
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
std::vector<Block*> Snake::getSnake(){
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
    Block* head = snake[0];   
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
    std::cout << "--------- snake's block ---------" << std::endl;
    for(int i=0 ; i<snake.size() ; i++){        
        std::cout << "block " << i << ": posX: " << snake[i]->posx << " posY: " << 
        snake[i]->posy << " dir: " << snake[i]->dir << std::endl;
    }
    for(int i=0 ; i<dirs.size() ; i++){
        std::cout << "dir" << i << " = " << dirs[i] << std::endl;
    }
}

//Playground::Playground(int blockWidth, int blockHeight, int width, int height):win(window),ren(renderer),width(width),height(height)
Playground::Playground(SDL_Window* window, SDL_Renderer* renderer, int width, int height):win(window),ren(renderer),width(width),height(height){
    int w,h;
    SDL_GetWindowSize(window,&w, &h);
    rectWidth = w/width;
    rectHeight = h/height;   
    for(int y=0 ; y<height ; y++){
        for(int x=0 ; x<width ; x++){
            pixels.push_back(new Pixel(x,y,false,RPLAY,GPLAY,BPLAY));
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
            // SDL_SetRenderDrawColor(ren, LC, LC, LC, 255);
            // SDL_RenderDrawRect(ren,&rect);  
        }else{
            SDL_SetRenderDrawColor(ren, RPLAY, GPLAY, BPLAY, 255); 
            SDL_RenderFillRect(ren,&rect);
            // SDL_SetRenderDrawColor(ren, LC, LC, LC, 255);
            // SDL_RenderDrawRect(ren,&rect);  
        }
    }
    SDL_RenderPresent(ren);
}

Collision Playground::getCollision(Snake* snake, Block* target){
    Block* head = snake->getHead();
    std::vector<Block*> s = snake->getSnake();
    if((head->posx==target->posx)&&(head->posy==target->posy)) return TARGET;

    for(int i=1 ; i<snake->getSnake().size() ; i++){   
        if((head->posx==s[i]->posx)&&(head->posy==s[i]->posy)) return SNAKE;  
        if((s[i]->posx>width)||(s[i]->posx<0)||(s[i]->posy>height)||(s[i]->posy<0)) return WALL;   
    }  
    
    return NONE;
}
bool Playground::isTargetPosOK(Snake* snake, Block* target){
    std::vector<Block*> sn = snake->getSnake();
    for(auto s : sn){
        if((s->posx==target->posx)&&(s->posy==target->posy)) return false;
    }
    return true;
}

void border1(Snake* s, int w, int h){
    for(auto s : s->getSnake()){
        s->posx = (s->posx<0) ? w-1 : s->posx;
        s->posx = (s->posx>w-1) ? 0: s->posx;
        s->posy = (s->posy<0) ? h-1 : s->posy;
        s->posy = (s->posy>h-1) ? 0: s->posy;
    }   
}
void border2(Snake* s, int w, int h){
    for(auto s : s->getSnake()){
        if((s->posx<0)&&(s->posy<0)){
            s->posx = w-1;
            s->posy = h-1;
        }
        if((s->posx<0)&&(s->posy>h-1)){
            s->posx = w-1;
            s->posy = 0;
        }
        if((s->posx>w-1)&&(s->posy<0)){
            s->posx = 0;
            s->posy = h-1;
        }
        if((s->posx>w-1)&&(s->posy>h-1)){
            s->posx = 0;
            s->posy = 0;
        }

    }   
}
void Playground::bordersManagement(Snake* s, int difficulty){
    // border1(s,width,height);
    // border2(s,width,height);
    switch(difficulty){
        case 0:  break;
        case 1: border1(s, width, height); break;
    }   
}

// this function should update pixels with the snake's position
void Playground::update(Snake* snake){  
    snake->updatePos();  
    snake->updateDir();
    bordersManagement(snake,1); // need difficulty level from main
#ifdef DEBUG
    snake->debug();
#endif
    for(auto p: pixels){
        p->activ=false;
    }
    for(auto s: snake->getSnake()){
        pixels[s->posx+s->posy*width]->activ = s->activ;
        pixels[s->posx+s->posy*width]->r = s->r;
        pixels[s->posx+s->posy*width]->g = s->g;
        pixels[s->posx+s->posy*width]->b = s->b;        
    }
     
}
// FIX THIS METHOD !! 
// TARGET IMPACT SNAKE'S RENDERING AND SOME BLOKS STAY ACTIVE
void Playground::update(Snake* snake, Block* target){  
    snake->updatePos();  
    snake->updateDir();
    bordersManagement(snake,1); // need difficulty level from main
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
        pixels[s->posx+s->posy*width]->activ = s->activ;
        pixels[s->posx+s->posy*width]->r = s->r;
        pixels[s->posx+s->posy*width]->g = s->g;
        pixels[s->posx+s->posy*width]->b = s->b;        
    }
}