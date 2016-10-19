#include <iostream>
#include "SDL.h"
#include <vector>

//using namespace std;

#define fps 60
#define window_width 400
#define window_hight 200

void cap_framerate(Uint32 starting_tick){
  if((1000/fps) > SDL_GetTicks() - starting_tick)
    SDL_Delay(1000/fps - (SDL_GetTicks() - starting_tick));
}

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


class SpriteGroup{
private:

  // pointer, since we don't want a copy of the object but the object itself
  std::vector<Sprite*> sprites;

  int sprites_size;

public:
  SpriteGroup copy(){
    SpriteGroup new_group;

    for(int i = 0; i < sprites.size(); i++){
      new_group.add(sprites[i]);
    }

    return new_group;
  }

  void add(Sprite *sprite){
    sprites.push_back(sprite);
    sprites_size = sprites.size();
  }

  void remove(Sprite sprite_object){

    for(int i=0; i < sprites_size; i++)
      if( *sprites[i] == sprite_object)
        sprites.erase(sprites.begin() + i);

    sprites_size = sprites.size();
  }

  bool has(Sprite sprite_object){
    for(int i=0; i < sprites_size; i++)
      if( *sprites[i] == sprite_object)
        return true;
    return false;
  }


  void update(){
    if(!sprites.empty())
      for(int i=0; i < sprites_size; i++)
        sprites[i]->update();
  }

  void draw(SDL_Surface *destination){
    if(!sprites.empty())
      for(int i=0; i < sprites_size; i++)
        sprites[i]->draw(destination);
  }

  void empty(){
    sprites.clear();
    sprites_size = sprites.size();
  }

  int size(){
    return sprites_size;
  }

  std::vector<Sprite*> get_sprites(){
    return sprites;
  }
};


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
      loaded_image = SDL_LoadBMP(filename);

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
