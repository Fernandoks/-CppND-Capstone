/*******************************************************************************
* Title                 :   Snake Game
* Filename              :   Controller.cpp
* Author                :   Fernando Kaba Surjus
* Origin Date           :   05/06/2020
* Version               :   1.0.0
* Compiler              :   GNU G++ 
* Target                :   Linux
* Notes                 :   None
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESSED
* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE AUTHOR OR ITS CONTRIBUTORS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
*******************************************************************************/
/*************** SOURCE REVISION LOG *****************************************
*
*    Date    Version   Author             Description 
*  04/06/20  1.0.0   Fernando Kaba Surjus  Initial Release.
*
*******************************************************************************/
/** @file TODO: Controller.cpp
 *  @brief Controller Class - responsible to detect the keyboard press event
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include "renderer.h"
#include "controller.h"
#include <iostream>
#include "SDL.h"


/******************************************************************************
* Methods Definitions
*******************************************************************************/
void Controller::ChangeDirection(Snake &snake, Snake::Direction input,
                                 Snake::Direction opposite) const {
  if (snake.direction != opposite || snake.size == 1) snake.direction = input;
  return;
}

void Controller::HandleInput(bool &running, Snake &snake) 
{
  SDL_Event e;
  while (SDL_PollEvent(&e)) 
  {
    if (e.type == SDL_QUIT) 
    {
      running = false;
    } 
    else if (e.type == SDL_KEYDOWN) 
    {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          ChangeDirection(snake, Snake::Direction::kUp,
                          Snake::Direction::kDown);
          break;

        case SDLK_DOWN:
          ChangeDirection(snake, Snake::Direction::kDown,
                          Snake::Direction::kUp);
          break;

        case SDLK_LEFT:
          ChangeDirection(snake, Snake::Direction::kLeft,
                          Snake::Direction::kRight);
          break;

        case SDLK_RIGHT:
          ChangeDirection(snake, Snake::Direction::kRight,
                          Snake::Direction::kLeft);
          break;

        //Including a pause key
        case SDLK_ESCAPE:
            if (_paused == false) Pause();
            else Resume();
         break; 
         //Including W key for toggle wall
        case SDLK_w:
          _wall ? _wall = false : _wall = true;
        break;
      }
    }
  }
}

// Pausing

bool Controller::IsPaused()
{
  return _paused;
}

void Controller::Pause()
{
  this->_paused = true;
}

void Controller::Resume()
{
  this->_paused = false;
}