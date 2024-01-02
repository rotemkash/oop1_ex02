#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Demon.h"
#include "PacMan.h"
#include "io.h"

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;

#define WALL '#'
#define DOOR 'D'
#define KEY '%'
#define DEMON '&'
#define GIFT '$'
#define COOKIE '*'
#define PACMAN 'a'
#define PACMAN_SUP '@'
#define SPACE ' '

const int EATEN = -2;
const int ILLEGAL_MV = -1;
const int NORMAL_MV = 0;
const int ATE_COOKIE = 2;
const int ATE_GIFT = 5;
const int ATE_KEY = 7;

class Board {
 public:
  Board(ifstream &file);

  int cookieCount();
  int demonCount();
  bool moveDemons();

  int movePacUp();
  int movePacDown();
  int movePacLeft();
  int movePacRight();

  void resetBoard();
  void printBoard();

 private:
  int _rows;
  int _cols;
  PacMan _pacman;
  vector<Demon> _demons;
  vector<Location> _door_locs;
  vector<Location> _cookie_locs;
  vector<Location> _key_locs;
  vector<Location> _gift_locs;
  vector<string> _board;

  void getObjPositions();

  void ReadFile(ifstream &file);
  int updatePacLoc(int newRow, int newCol);

  bool moveHorizantalDemon(Demon &g);
  bool moveVerticalDemon(Demon &g);
  bool moveDrunkDemon(Demon &g);

  bool moveDemonUp(Demon &g);
  bool moveDemonDown(Demon &g);
  bool moveDemonLeft(Demon &g);
  bool moveDemonRight(Demon &g);

  bool isMoveValid(int newRow, int newCol);

  void keyEaten(int newRow, int newCol);

  void giftEaten(int newRow, int newCol);

  void cookieEaten(int newRow, int newCol);

  void breakDoor(int newRow, int newCol);

  void updateObjs(vector<Location> locVec, char ch);
  void clearObj();
};