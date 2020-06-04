#include "game.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width)),
      random_h(0, static_cast<int>(grid_height)), 
      _badfood(false) {
  PlaceFood();
}

Game::~Game()
{

}

void Game::Run(Controller &controller, Renderer &renderer,
               std::size_t target_frame_duration) 
{
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) 
  {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update(controller, renderer);
    renderer.Render(snake, food, this->IsBadFood());

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) 
    {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }
      
    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) 
    {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  // Decide if is _badfood
  std::random_device myrand;
  std::mt19937 mteng(myrand());
  std::uniform_int_distribution<int> mydis(1,10);
  auto ranvalue = mydis(mteng);
  
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) 
    {
      if (ranvalue <= 5)
      {
        _badfood = true;
        std::thread poisonTimer(TimerThread, &_badfood);
        poisonTimer.detach();
      }
      else 
      {
        _badfood = false;
      }
      food.x = x;
      food.y = y;
      return;
    }
  }
}

// Clear the poisoned food after 3 seconds
void TimerThread(bool *poisoned) {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    *poisoned = false;
}

void Game::Update(Controller &controller, Renderer &renderer) 
{
  //Checks if the game is paused
  if (controller.IsPaused() == true)
  {
    renderer.PauseTitle();
    return;
  }

  if (!snake.alive) 
  {
    effect.load("../sounds/gameover.wav");
    effect.play();
    return;
  }

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) 
  {
    if (_badfood == true)
    {
      score--;
      effect.load("../sounds/error.wav");
      effect.play();
      PlaceFood();
      // Grow snake and increase speed.
      snake.ReduceBody(); 
      snake.speed += 0.02;
    }
    else 
    {
      score++;
      effect.load("../sounds/success.wav");
      effect.play();
      PlaceFood();
      // Grow snake and increase speed.
      snake.GrowBody();
      snake.speed += 0.02;
    }
    
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }

