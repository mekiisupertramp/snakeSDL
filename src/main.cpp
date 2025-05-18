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
//#include <SDL2/SDL_main.h>
#include "playground.hpp"

#define WIDTH   15
#define HEIGHT  15
#define PIXELSX 600
#define PIXELSY 600

using namespace std;


int main(int argc, char* argv[]) {
    // Initialize SDL's video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow(
        "Snake",             // window title
        SDL_WINDOWPOS_CENTERED,           // initial x position
        SDL_WINDOWPOS_CENTERED,           // initial y position
        PIXELSX,                              // width, in pixels
        PIXELSY,                              // height, in pixels
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

    // Set draw color to black (R, G, B, A)
    SDL_SetRenderDrawColor(renderer, 50, 0, 0, 255);

    // Clear the window (fills it with the draw color)
    SDL_RenderClear(renderer);

    // Show the cleared window
    SDL_RenderPresent(renderer);

    int difficulty=2;

    Playground* play = new Playground(window,renderer,WIDTH,HEIGHT,difficulty);
    Snake* snake = new Snake();
    int points = 0;
    std::srand(std::time(nullptr)); 
    Block* b = new Block(std::rand() % WIDTH, std::rand() % HEIGHT);
    b->activ = true;
    b->r = RTARG;
    b->g = GTARG;
    b->b = BTARG;
    snake->addBlock();
    snake->addBlock();
    play->update(snake);

    SDL_SetRenderDrawColor(renderer, 50, 0, 0, 255);
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
                    b->posx = std::rand() % WIDTH;
                    b->posy = std::rand() % HEIGHT;
                }
                cout << "target x: " << b->posx << ", y: " << b->posy << endl;
            }else{
                if(play->getCollision(snake,b) == SNAKE) {
                    cout << "Game over!" << endl;
                    cout << "You won " << points << " points." << endl;
                    running = false;
                }
            }
            play->render();               
            cpt=0;
        }   
        cpt++;        
        SDL_Delay(25);  // milliseconds
    }    

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}