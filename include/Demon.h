#pragma once

// #include "Vertex.h"
#include "Location.h"
enum Behavior {
  HORIZANTAL = 0,
  VERTICAL = 1,
  DRUNK = 2,
};

class Demon {
 public:
  Demon(Location loc, Behavior behave, bool flag);
  void set_pos(Location new_pos);
  Location get_pos();
  void resetPos();

  void moveUp();
  void moveDown();
  void moveLeft();
  void moveRight();
  Behavior getBehavior();
  bool isFlag();
  void setFlag();
  void resetFlag();

 private:
  Location _pos;
  Location _init_pos;
  Behavior _behave;
  bool _flag;
  // Location _init_pos;
};
