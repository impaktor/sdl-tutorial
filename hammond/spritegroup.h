#ifndef SPRITEGROUP_H
#define SPRITEGROUP_H

#include <vector>
#include "SDL.h"

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

#endif
