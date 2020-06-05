#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"

class Controller {
 public:
  void HandleInput(bool &running, Snake &snake);
  bool IsPaused();
  bool IsWall() {return _wall;}

 private:
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;

  void Resume();
  void Pause();
  

  mutable bool _paused;
  bool _wall = false;

};

#endif