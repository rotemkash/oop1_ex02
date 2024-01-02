#pragma once

#include <iostream>

#include "Board.h"

const int LVL_BONUS = 50;
const int DEMON_BONUS = 2;
const int LVL_COUNT = 3;
const int INIT_HP = 3;
const int INIT_SCORE = 0;
const int INIT_LVL = 1;
const int EXIT = -3;
const int ILLEGAL_KB = -1;

enum Keys {
  KB_Escape = 27,
  SpecialKey = 224,
};

enum SpecialKeys {
  KB_Up = 72,
  KB_Down = 80,
  KB_Left = 75,
  KB_Right = 77,
};

class GameRunner {
 public:
  GameRunner();
  void Run();

 private:
  int _hp;
  int _score;
  int _level;
  ifstream _game_file;

  void open_file(ifstream &file);
  void printGame(Board board);
  int handleSpecialKey(Board &board);
};