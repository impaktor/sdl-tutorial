#ifndef SPRITE_H
#define SPRITE_H

#include "SDL_image.h"
#include "SDL.h"

class Sprite{
protected:
  SDL_Surface *image;
  SDL_Rect rect;
  int origin_x, origin_y;
public:
  Sprite(Uint32 color, int x, int y, int w = 48, int h = 64){
    image = SDL_CreateRGBSurface(0, w, h, 32, 0,0,0,0);
    SDL_FillRect(image, NULL, color);
    rect = image->clip_rect;

    origin_x = rect.x / 2.0; // put in centre of screen
    origin_y = rect.y / 2.0;
    rect.x = x; // - origin_x; (do this in the inheriting classes instead)
    rect.y = y; // - origin_y;
  }

  void update(){
    //can be overridden

  }

  void draw(SDL_Surface *destination){
    SDL_BlitSurface(image, NULL, destination, &rect);
  }

  SDL_Surface* get_image() const{
    return image;
  }

  bool operator==(const Sprite &other) const{
    // SDL has defined this operation, so is OK, checks if place in
    // memory is the same
    return (image == other.get_image());
  }

};

#endif
