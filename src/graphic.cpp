#include "graphic.hpp"

Graphic::Graphic(SDL_Window* window, SDL_Renderer* renderer) : window(window), renderer(renderer){
    // Set draw color to black (R, G, B, A)
    SDL_SetRenderDrawColor(renderer, 50, 0, 0, 255);
    // Clear the window (fills it with the draw color)
    SDL_RenderClear(renderer);
    // Show the cleared window
    SDL_RenderPresent(renderer);  
}

// Graphic::Graphic(std::string name, int widthPix, int heigthPix){

//     // // Initialize SDL's video subsystem
//     // if (SDL_Init(SDL_INIT_VIDEO) != 0) {
//     //     std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
//     // }

//     // SDL_Window* window = SDL_CreateWindow(
//     //     name.c_str(),              // window title
//     //     SDL_WINDOWPOS_CENTERED,     // initial x position
//     //     SDL_WINDOWPOS_CENTERED,     // initial y position
//     //     widthPix,                   // width, in pixels
//     //     heigthPix,                  // height, in pixels
//     //     SDL_WINDOW_SHOWN            // flags
//     // );
//     // if (!window) {
//     //     std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
//     //     SDL_Quit();
//     // }

//     // // Create a renderer (hardware-accelerated)
//     // SDL_Renderer* renderer = SDL_CreateRenderer(
//     //     window, -1,
//     //     SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
//     // );
//     // if (!renderer) {
//     //     std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
//     //     SDL_DestroyWindow(window);
//     //     SDL_Quit();
//     // }

//     // // Set draw color to black (R, G, B, A)
//     // SDL_SetRenderDrawColor(renderer, 50, 0, 0, 255);

//     // // Clear the window (fills it with the draw color)
//     // SDL_RenderClear(renderer);

//     // // Show the cleared window
//     // SDL_RenderPresent(renderer);

// }

Graphic::~Graphic(){}    
void Graphic::render(std::vector<Square*> pixels, int width, int height, int box){   
    int x, y;
    SDL_GetWindowSize(window, &x, &y);
    int pixWidth = x/width;
    int pixHeight = y/height;

    SDL_Rect rect = {.x=0, .y=0, .w=pixWidth, .h=pixHeight};
    for(auto p : pixels){
        rect.x = p->posx*pixWidth;
        rect.y = p->posy*pixHeight;
        if(p->activ){            
            SDL_SetRenderDrawColor(renderer, p->r, p->g, p->b, 255); 
            SDL_RenderFillRect(renderer,&rect);
            SDL_SetRenderDrawColor(renderer, LC, LC, LC, 255);
            SDL_RenderDrawRect(renderer,&rect);  
        }else{
            SDL_SetRenderDrawColor(renderer, RPLAY, GPLAY, BPLAY, 255); 
            SDL_RenderFillRect(renderer,&rect);
            SDL_SetRenderDrawColor(renderer, LC, LC, LC, 255);
            SDL_RenderDrawRect(renderer,&rect);  
        }
    }
    if(box == 0){
        rect.x=0; rect.y=0;
        SDL_GetWindowSize(window, &rect.w, &rect.h);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
        SDL_RenderDrawRect(renderer,&rect);
        rect.x=1; rect.y=1;
        rect.w -= 2;
        rect.h -=2;
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
        SDL_RenderDrawRect(renderer,&rect);
    }
    SDL_RenderPresent(renderer);
}
void Graphic::render(Playground* play){   
    int x, y;
    SDL_GetWindowSize(window, &x, &y);

    SDL_Rect rect = {.x=0, .y=0, .w=x/play->getWidth(), .h=y/play->getHeigth()};
    for(auto p : play->getPixels()){
        rect.x = p->posx*x/play->getWidth();        
        rect.y = p->posy*y/play->getHeigth();
        if(p->activ){            
            SDL_SetRenderDrawColor(renderer, p->r, p->g, p->b, 255); 
            SDL_RenderFillRect(renderer,&rect);
            SDL_SetRenderDrawColor(renderer, LC, LC, LC, 255);
            SDL_RenderDrawRect(renderer,&rect);  
        }else{
            SDL_SetRenderDrawColor(renderer, RPLAY, GPLAY, BPLAY, 255); 
            SDL_RenderFillRect(renderer,&rect);
            SDL_SetRenderDrawColor(renderer, LC, LC, LC, 255);
            SDL_RenderDrawRect(renderer,&rect);  
        }
    }
    if(play->getDifficulty() == 0){
        rect.x=0; rect.y=0;
        rect.w = play->getWidth()*x/play->getWidth();
        rect.h = play->getHeigth()*y/play->getHeigth();
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
        SDL_RenderDrawRect(renderer,&rect);
        rect.x=1; rect.y=1;
        rect.w = (play->getWidth()*x/play->getWidth())-2;
        rect.h = (play->getHeigth()*y/play->getHeigth())-2;
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
        SDL_RenderDrawRect(renderer,&rect);
    }
    SDL_RenderPresent(renderer);
}