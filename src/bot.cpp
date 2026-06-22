#include "bot.hpp"
#include <cstdlib>
#include <vector>

static bool isReverse(Direction current, Direction next){
    return ((current == TOP) && (next == BOTTOM)) ||
           ((current == BOTTOM) && (next == TOP)) ||
           ((current == LEFT) && (next == RIGHT)) ||
           ((current == RIGHT) && (next == LEFT));
}

static Square nextHead(Square* head, Direction dir){
    Square next(head->posx, head->posy, dir);
    switch(dir){
        case TOP: next.posy--; break;
        case BOTTOM: next.posy++; break;
        case LEFT: next.posx--; break;
        case RIGHT: next.posx++; break;
    }
    return next;
}

static bool hitsSnake(const Square& head, Snake* snake){
    for(auto s : snake->getSnake()){
        if((head.posx == s->posx) && (head.posy == s->posy)) return true;
    }
    return false;
}

static bool isSafe(Direction dir, Snake* bot, Snake* player, Playground* play){
    Square candidate = nextHead(bot->getHead(), dir);
    if((candidate.posx < 0) || (candidate.posx >= play->getWidth())) return false;
    if((candidate.posy < 0) || (candidate.posy >= play->getHeigth())) return false;

    const std::vector<Square*>& botBody = bot->getSnake();
    for(int i=1 ; (size_t)i<botBody.size() ; i++){
        if((candidate.posx == botBody[i]->posx) && (candidate.posy == botBody[i]->posy)) return false;
    }

    return !hitsSnake(candidate, player);
}

Direction getBotNextDirection(Snake* bot, Snake* player, Square* target, Playground* play){
    Square* head = bot->getHead();
    std::vector<Direction> preferred;

    if(target->posx < head->posx) preferred.push_back(LEFT);
    if(target->posx > head->posx) preferred.push_back(RIGHT);
    if(target->posy < head->posy) preferred.push_back(TOP);
    if(target->posy > head->posy) preferred.push_back(BOTTOM);

    preferred.push_back(TOP);
    preferred.push_back(BOTTOM);
    preferred.push_back(LEFT);
    preferred.push_back(RIGHT);

    for(auto dir : preferred){
        if(isReverse(head->dir, dir)) continue;
        if(isSafe(dir, bot, player, play)) return dir;
    }

    return head->dir;
}
