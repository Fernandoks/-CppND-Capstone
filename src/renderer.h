#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"
#include <SDL2/SDL_ttf.h> //libsdl2-ttf-dev
#include <SDL2/SDL_image.h> //libsdl2-image-dev


class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(Snake const &snake, SDL_Point const &food, bool isBadFood, int score, bool wall);
  void UpdateWindowTitle(int score, int fps);

  void PauseTitle();

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;

  TTF_Font *font;
  SDL_Surface *surfaceMessage;
  SDL_Texture *Message;
  SDL_Texture *texture;
  SDL_Surface *imageSurface;
  SDL_Surface *windowSurface;
  SDL_Surface *optimizedImg;

  SDL_Color White = {255, 255, 255};
  SDL_Color Red = {255, 0, 0};;
  SDL_Rect Message_rect;

};

#endif