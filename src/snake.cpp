/*******************************************************************************
* Title                 :   Snake Game
* Filename              :   snake.cpp
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
/** @file TODO: snake.cpp
 *  @brief Snake Class - responsible to controle the Snake body
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include "snake.h"
#include <cmath>
#include <iostream>

/******************************************************************************
* Methods Definitions
*******************************************************************************/
void Snake::Update(bool wall) {
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(
          head_y)};  // We first capture the head's cell before updating.
  UpdateHead(wall);
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

void Snake::UpdateHead(bool wall) 
{
  switch (direction) 
  {
    case Direction::kUp:
      head_y -= speed;
      break;

    case Direction::kDown:
      head_y += speed;
      break;

    case Direction::kLeft:
      head_x -= speed;
      break;

    case Direction::kRight:
      head_x += speed;
      break;
  }
  // check if the wall is active
  if(wall == true){
    // check if snake hit the wall
    if(head_x > 32 || head_y > 32 || head_x < 0 || head_y < 0){
      std::cout << "you hit the wall" << std::endl;
      alive = false;
    
    }    
  }
  else
  {
    // Wrap the Snake around to the beginning if going off of the screen.
    // if there is no wall
    head_x = fmod(head_x + grid_width, grid_width);
    head_y = fmod(head_y + grid_height, grid_height);
  }
}


void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  if (growing) 
  {
    body.push_back(prev_head_cell);
    growing = false;
    size++;  
  } 
  else if (_reducing)
  {
    //body.erase(body.begin());
    body.pop_back();
    body.push_back(prev_head_cell);
    body.erase(body.begin());
    _reducing = false;
    size--; 
    if (size <= 1)
    {
      alive = false;
      return;
    }
  }
  else 
  {
    // Remove the tail from the vector.
    body.push_back(prev_head_cell);
    body.erase(body.begin());
  }

  // Check if the snake has died.
  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      alive = false;
    }
  }
}

void Snake::GrowBody() { growing = true; _reducing = false; }
void Snake::ReduceBody() { _reducing = true; growing = false;}

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}