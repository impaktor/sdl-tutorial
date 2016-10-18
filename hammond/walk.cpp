#include <iostream>
#include "SDL.h"

using namespace std;

#define fps 60
#define window_width 400
#define window_hight 200

void cap_framerate(Uint32 starting_tick){
  if((1000/fps) > SDL_GetTicks() - starting_tick)
    SDL_Delay(1000/fps - (SDL_GetTicks() - starting_tick));
}

class Sprite{
private:
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
    rect.x = x - origin_x;
    rect.y = y - origin_y;
  }

  void update(){
    //can be overridden

  }

  void draw(SDL_Surface *destination){
    SDL_BlitSurface(image, NULL, destination, &rect);
  }


};

int main(int argc, char *argv[])
{
  SDL_Init(SDL_INIT_EVERYTHING);

  // ceate a window
  SDL_Window *window;
  window = SDL_CreateWindow("Walk on base",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            window_width,
                            window_hight,
                            SDL_WINDOW_RESIZABLE);


  if(window == NULL){
    cout << "Error initializing window: " << endl
         << SDL_GetError() << endl;
  }

  SDL_Surface *screen = SDL_GetWindowSurface(window);
  Uint32 white = SDL_MapRGB(screen->format, 255, 255, 255);
  Uint32 red = SDL_MapRGB(screen->format, 255, 0, 0);
  SDL_FillRect(screen, NULL, white);


  // Create a sprite, and draw to screen:
  Sprite object(red, window_width/2.0, window_hight/2.0);
  object.draw(screen);

  // Put the white on the actual window (don't need to do this every tic, so outside of loop)
  SDL_UpdateWindowSurface(window);



  SDL_Event event;
  bool is_running = true;
  Uint32 starting_tick;

  while(is_running){
    starting_tick = SDL_GetTicks();

    while(SDL_PollEvent(&event)){
      if(event.type == SDL_QUIT){
        is_running = false;
        break;
      }
    }

    cap_framerate(starting_tick);
  }



  //SDL_Delay(3000);

  SDL_DestroyWindow(window);

  SDL_Quit();
  return 0;
}



