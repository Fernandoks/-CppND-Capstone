#include "game.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <future>

#define WAVSUCCESS_PATH "../sounds/success.wav"
#define WAVERROR_PATH "../sounds/error.wav"
#define WAVGAMEOVER_PATH "../sounds/gameover.wav"
#define WAVSTART_PATH "../sounds/start.wav"


Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width)),
      random_h(0, static_cast<int>(grid_height)), 
      _badfood(false) 
{
  PlaceFood();
  Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512);
  Mix_AllocateChannels(4);
  MixSuccess = Mix_LoadWAV(WAVSUCCESS_PATH);
  MixError = Mix_LoadWAV(WAVERROR_PATH);
  MixStart = Mix_LoadWAV(WAVSTART_PATH);
  MixGameOver = Mix_LoadWAV(WAVGAMEOVER_PATH);
 
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

  std::string msgText{"Click to START your game. Use ESC to pause, and W to include a Wall"};
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "START", msgText.c_str(), NULL);

  Mix_PlayChannel(-1, MixStart, 0);

  while (running) 
  {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update(controller, renderer);
    renderer.Render(snake, food, this->IsBadFood(),score, controller.IsWall());

    if (!snake.alive) 
    {
      Mix_PlayChannel(-1, MixGameOver, 0);
      // show a dialog box
      std::string msgBoxText{"Score: " + std::to_string(GetScore()) + "\n Size: " + std::to_string(GetSize())};
      SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "You died!", msgBoxText.c_str(), NULL);
      return;
    }

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) 
    {
      renderer.UpdateWindowTitle(score, frame_count);
      //renderer.DisplayText();
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
      food.x = x;
      food.y = y;
      SetGoodFood();
      if (ranvalue <= 5)
      {
        SetBadFood();
        std::thread poisonTimer(&Game::TimedThread, this);
        poisonTimer.detach();
      }
      return;
    }
  }
}



void Game::Update(Controller &controller, Renderer &renderer) 
{
    //Checks if the game is paused
  if (controller.IsPaused() == true)
  {
    renderer.PauseTitle();
    return;
  }
  
  snake.Update(controller.IsWall());

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) 
  {
    if (IsBadFood() == true)
    {
      score--;
      PlaceFood();
      // Grow snake and increase speed.
      snake.ReduceBody(); 
      snake.speed -= 0.02;
      Mix_PlayChannel(-1, MixError, 0);
    }
    else 
    {
      score++;
      PlaceFood();
      // Grow snake and increase speed.
      snake.GrowBody();
      snake.speed += 0.02;
      Mix_PlayChannel(-1, MixSuccess, 0);
    }
    
  }
}
void Game::SetBadFood()
{
  std::lock_guard<std::mutex> lck(_mutex);
  _badfood = true;

}

void Game::SetGoodFood()
{
  std::lock_guard<std::mutex> lck(_mutex);
  _badfood = false;
}

bool Game::IsBadFood()
{
  std::lock_guard<std::mutex> lck(_mutex);
  return _badfood;
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }

void Game::TimedThread()
{
  std::this_thread::sleep_for(std::chrono::seconds(3));
  SetGoodFood();
}

