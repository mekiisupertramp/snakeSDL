// 
//  graphic.hpp
//

#ifndef graphic_hpp
#define graphic_hpp

#include <string>
#include <vector>
#include "playground.hpp"
#include <SDL2/SDL.h>
#include "square.hpp"

class Graphic{
    private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    public:
    Graphic(std::vector<Square*> pixels, int width, int height, int dif);
    Graphic(SDL_Window* window, SDL_Renderer* renderer);
    ~Graphic();    
    void render(std::vector<SDL_Rect> rects);
    void render(std::vector<Square*> pixels, int pixWidth, int pixHeight, int dif);
    void render(Playground* play);

    void quit();
};


#endif /* graphic_hpp */