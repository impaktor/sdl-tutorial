#include <iostream>
#include "SDL_mixer.h"
#include "SDL_image.h"
#include "SDL.h"
#include <vector>
#include "sprite.h"
#include "spritegroup.h"

//using namespace std;

#define fps 60
#define window_width 400
#define window_hight 200

void cap_framerate(Uint32 starting_tick){
  if((1000/fps) > SDL_GetTicks() - starting_tick)
    SDL_Delay(1000/fps - (SDL_GetTicks() - starting_tick));
}


class Block : public Sprite {
public:
  Block(Uint32 color, int x, int y, int w = 48, int h = 64) :
    Sprite(color, x, y, w, h){
    update_properites();
  }

  void update_properites(){
    origin_x = 0;
    origin_y = 0;

    set_position(rect.x, rect.y);
  }

  void set_position(int x, int y){
    rect.x = x - origin_x;
    rect.y = y - origin_y;
  }

  void set_image(const char filename[] = NULL){
    if( filename != NULL){
      SDL_Surface *loaded_image = NULL;
      loaded_image = IMG_Load(filename);

      if(loaded_image != NULL){
        image = loaded_image;

        // Note, setting the image, resets the SDL rect, which we
        // don't want, so:
        int old_x = rect.x;
        int old_y = rect.y;
        rect = image->clip_rect;
        rect.x = old_x;
        rect.y = old_y;

        update_properites();
      }
    }
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
    std::cout << "Error initializing window: " << std::endl
              << SDL_GetError() << std::endl;
  }

  SDL_Surface *screen = SDL_GetWindowSurface(window);
  Uint32 white = SDL_MapRGB(screen->format, 255, 255, 255);
  Uint32 red = SDL_MapRGB(screen->format, 255, 0, 0);
  Uint32 blue = SDL_MapRGB(screen->format, 0, 0, 255);
  SDL_FillRect(screen, NULL, white);


  Block block(red, 0,0);
  block.set_image("sprite.bmp");

  // // Create a sprite, and draw to screen:
  // Sprite object(red, window_width/2.0, window_hight/2.0);
  // Sprite another(blue, window_width/2.0 -100, window_hight/2.0 +20);

  SpriteGroup active_sprites;
  active_sprites.add(&block);

  active_sprites.draw(screen);
  //  object.draw(screen);

  // Put the white on the actual window (don't need to do this every tic, so outside of loop)
  SDL_UpdateWindowSurface(window);

  //Initialize SDL2_mixer (Don't use SDL_OpenAudio(), but the Mix_OpenAudio)
  Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096);
  Mix_Chunk *sound = NULL;
  sound = Mix_LoadWAV("sound.wav");
  Mix_PlayChannel(-1, sound, 0);

  // Now play around with palying music, using SDL2_mixer
  Mix_Music *music = NULL;
  music = Mix_LoadMUS("music.wav");
  Mix_PlayMusic(music, -1);

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


  Mix_FreeChunk(sound);
  sound = NULL;

  //SDL_Delay(3000);
  Mix_CloseAudio();
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
