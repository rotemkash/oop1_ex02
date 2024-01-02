#pragma once

#include "Location.h"

#define SUPER_TIME 20

class PacMan {
 public:
  PacMan();
  void set_pos(Location new_pos);
  Location get_pos();
  void set_init_pos(Location pos);
  Location get_init_pos();
  void resetPos();

  void moveUp();
  void moveDown();
  void moveLeft();
  void moveRight();
  bool isSuper();
  void setSuper();
  void decSuper();

 private:
  Location _pos;
  Location _init_pos;
  int _is_super;
};
