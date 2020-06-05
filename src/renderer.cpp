#include "renderer.h"
#include <iostream>
#include <sstream>
#include <string>


Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }   
  
  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
  {
    std::cerr << "SDL image could not initialize.\n";
  }
  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);
  if (nullptr == sdl_window) 
  {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }     

  //TFT
  TTF_Init();
  font = TTF_OpenFont("../SansBold.ttf", 24);
  if (font == NULL) {
      fprintf(stderr, "error: font not found\n");
      exit(EXIT_FAILURE);
  }

  //Background 
  windowSurface = SDL_GetWindowSurface( sdl_window );
  imageSurface = IMG_Load( "../surface.png");
  if( imageSurface == NULL )
  {
      std::cout << "SDL could not load image! SDL Error: " << SDL_GetError( ) << std::endl;
  }

  optimizedImg = SDL_ConvertSurface(imageSurface, windowSurface->format, 0);
  if (optimizedImg == NULL)
  {
    std::cerr << "SDL fail to optimize image.\n";
  }
  SDL_FreeSurface(imageSurface);
  
  
  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_FreeSurface(surfaceMessage);
  SDL_FreeSurface(imageSurface);
  SDL_FreeSurface(windowSurface);
  SDL_FreeSurface(optimizedImg);
  
  SDL_DestroyTexture(Message);
  SDL_DestroyTexture(texture);
  
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render(Snake const &snake, SDL_Point const &food, bool isBadFood, int score, bool wall) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;
  
  // Clear screen
  
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
   
  SDL_RenderClear(sdl_renderer);
 
  texture = SDL_CreateTextureFromSurface(sdl_renderer, optimizedImg);
  SDL_RenderCopy(sdl_renderer, texture, NULL, NULL);
  
  if(wall == true)
  {
    SDL_SetRenderDrawColor(sdl_renderer , 0x7E, 0x41, 0x1F, 0xFF);  // brown
    SDL_Rect box = { 0 ,0 , 640 , 640}; // rectangle around the window
    SDL_RenderDrawRect(sdl_renderer , &box);
  }

  // Render food
  if (isBadFood == true)
  {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  else
  {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);//0xFF, 0xCC, 0x00, 0xFF);
  }
    block.x = food.x * block.w;
    block.y = food.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);

  
  // Render snake's body
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);

  //Text
  std::stringstream ss;
  ss << "Score:" << score;
  std::string s = ss.str();
  //Config message
  surfaceMessage = TTF_RenderText_Solid(font, s.c_str(), White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
  Message = SDL_CreateTextureFromSurface(sdl_renderer, surfaceMessage); //now you can convert it into a texture
  Message_rect.x = 10;  //controls the rect's x coordinate 
  Message_rect.y = 10; // controls the rect's y coordinte
  Message_rect.w = 70; // controls the width of the rect
  Message_rect.h = 30; // controls the height of the rect


  //Mind you that (0,0) is on the top left of the window/screen, think a rect as the text's box, that way it would be very simple to understand
  //Now since it's a texture, you have to put RenderCopy in your game loop area, the area where the whole code executes
  SDL_RenderCopy(sdl_renderer, Message, NULL, &Message_rect);
  
  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}

void Renderer::PauseTitle()
{
  std::string paused {"GAME PAUSED - to continue press ESC"};
  SDL_SetWindowTitle(sdl_window,paused.c_str());
}
