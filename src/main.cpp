//
//  main.cpp
//
//  Created by Mehmed Blazevic on 24.04.2025.
//

#include <iostream>
#include <unistd.h>
#include <vector>
#include <queue>
#include <math.h>
#include <SDL2/SDL.h>
#include <cstdlib>
#include <random>
#include <ctime>
#include "playground.hpp"
#include "graphic.hpp"


// default values
#define WIDTH   15
#define HEIGHT  15
#define PIXELSX 600
#define PIXELSY 600
#define SPEED   100
#define DIFFICULTY  3

using namespace std;

#define DEBUG


int main(int argc, char* argv[]) {
    int difficulty=DIFFICULTY;
    int w = WIDTH;
    int h = HEIGHT;
    int pixelsX=PIXELSX;
    int pixelsY=PIXELSY;
    int speed=SPEED;

    // manage program's parameters first
    if(argc > 1){
        if(argc == 7){                        
            w = atoi(argv[1]);
            h = atoi(argv[2]);
            pixelsX = atoi(argv[3]);
            pixelsY = atoi(argv[4]);
            speed = atoi(argv[5]);
            difficulty = atoi(argv[6]);
            cout << "w: " << w << " h: " << h << " pxsX: " << pixelsX << " pxsY: " << pixelsY << " speed: " << speed << " difficulty: " << difficulty << endl;
        }else{
            cout << "Here is the arguments you must use." << endl;
            cout << "./main <squares in x> <squares in y> <width in pixels> <height in pixels> <period in ms> <difficulty>" << endl;
            return 0;
        }
    }    

    // Initialize SDL's video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    //Create a window
    SDL_Window* window = SDL_CreateWindow(
        "Snake",             // window title
        SDL_WINDOWPOS_CENTERED,           // initial x position
        SDL_WINDOWPOS_CENTERED,           // initial y position
        pixelsX,                              // width, in pixels
        pixelsY,                              // height, in pixels
        SDL_WINDOW_SHOWN                  // flags
    );
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create a renderer (hardware-accelerated)
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
      

    Playground* play = new Playground(w,h,difficulty);
    Snake* snake = new Snake();
    Graphic* graph = new Graphic(window,renderer);
    int points = 0;
    std::srand(std::time(nullptr)); 
    Square* b = new Square(std::rand() % w, std::rand() % h);
    b->activ = true;
    b->r = RTARG;
    b->g = GTARG;
    b->b = BTARG;
    play->update(snake,b);


    SDL_Event e;
    bool running = true;
    int cpt=0;
    while (running) {
        while(SDL_PollEvent(&e)){
            switch(e.type){
                case SDL_QUIT: running = false; break;
                case SDL_KEYDOWN:
                if (e.key.repeat == 0) {
                    switch (e.key.keysym.sym) {
                        case SDLK_UP:
                            snake->setDir(TOP);
                            break;
                        case SDLK_DOWN:
                            snake->setDir(BOTTOM);
                            break;
                        case SDLK_LEFT:
                            snake->setDir(LEFT);
                            break;
                        case SDLK_RIGHT:
                            snake->setDir(RIGHT);
                            break;
                        default:
                            break;
                    }
                }
                break;
            }
        }
        if(cpt>5){
            play->update(snake,b);
            if(play->getCollision(snake,b) == TARGET){
                snake->addBlock(); 
                points++;         
                while(!play->isTargetPosOK(snake,b)){
                    b->posx = std::rand() % w;
                    b->posy = std::rand() % h;
                }
#ifdef DEBUG                
                cout << "target x: " << b->posx << ", y: " << b->posy << endl;
#endif
                graph->render(play->getPixels(),w,h,difficulty);
            }else{
                if(play->getCollision(snake,b) == SNAKE) {
                    cout << "Game over! You bit yourself!" << endl;
                    cout << "You won " << points << " points." << endl;
                    running = false;
                }else{
                    if(play->getCollision(snake,b) == WALL){
                        cout << "Game over! Wall touched!" << endl;
                        cout << "You won " << points << " points." << endl;
                        running = false;
                    }else{
                        graph->render(play->getPixels(),w,h,difficulty);
                    }
                }
            }              
            cpt=0;
        }   
        cpt++;        
        SDL_Delay(speed);  // milliseconds
    }    

    delete snake;
    delete play;
    delete b;
    
    // Clean up    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}