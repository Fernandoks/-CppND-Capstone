#ifndef GAME_H
#define GAME_H

#include <random>
#include <mutex>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL.h"


class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  bool IsBadFood();
  void SetBadFood();
  void SetGoodFood();

 private:
  Snake snake;
  SDL_Point food;
  
  //Music 
  Mix_Chunk *MixSuccess = NULL;
  Mix_Chunk *MixError = NULL;
  Mix_Chunk *MixGameOver = NULL;
  Mix_Chunk *MixStart = NULL;
  
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};
  bool _badfood;
  std::mutex _mutex;

  void PlaceFood();
  void Update(Controller &controller, Renderer &renderer);

  void TimedThread();
};

#endif