#include "Board.h"

/* Constructor of Board*/
Board::Board(ifstream &file) {
  ReadFile(file);
  getObjPositions();
};

/* return the count of cookies*/
int Board::cookieCount() { return (int)_cookie_locs.size(); }

/* retun the count of the demons */
int Board::demonCount() { return (int)_demons.size(); }

/*
 * This function loops around the board we have and creates all
 * the object of the game. if it finds:
 * PACMAN = 'a': creates the object of Pacman
 * DEMON = '&': add a demon to the demons vector
 * COOKIE = '*': add the location of the cookie to the cookie vector
 * DOOR = 'D': adds the door's location to the doors vector
 * KEY = '%': adds the key's location to the keys location vector
 * GIFT = '$': adds the gift's location to the gifts locations vector
 */
void Board::getObjPositions() {
  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _cols; j++) {
      switch (_board[i][j]) {
        case PACMAN: {
          _pacman.set_pos(Location(i, j));
          _pacman.set_init_pos(Location(i, j));
          break;
        }
        case DEMON: {
          Location g_loc = Location(i, j);
          Behavior behave = Behavior(rand() % 3);
          auto tmp_g = Demon(g_loc, behave, false);
          _demons.push_back(tmp_g);
          break;
        }
        case COOKIE: {
          Location c_loc = Location(i, j);
          _cookie_locs.push_back(c_loc);
          break;
        }
        case DOOR: {
          Location d_loc = Location(i, j);
          _door_locs.push_back(d_loc);
          break;
        }
        case KEY: {
          Location k_loc = Location(i, j);
          _key_locs.push_back(k_loc);
          break;
        }
        case GIFT: {
          Location gf_loc = Location(i, j);
          _gift_locs.push_back(gf_loc);
          break;
        }
        default: {
          continue;
        }
      }
    }
  }
}

//================ PACMAN MOVERS ===================

/*
 * This function moves the pacman up on the board
 */
int Board::movePacUp() {
  int points = 0;
  if (_pacman.get_pos().row - 1 > 0) {
    int oldRow = _pacman.get_pos().row, oldCol = _pacman.get_pos().col;

    if ((points = updatePacLoc(oldRow - 1, oldCol)) != ILLEGAL_MV) {
      _pacman.moveUp();
      return points;
    }
  }
  return points;
}

/*
 * This function moves the pacman down on the board
 */
int Board::movePacDown() {
  int points = 0;
  if (_pacman.get_pos().row + 1 < _rows) {
    int oldRow = _pacman.get_pos().row, oldCol = _pacman.get_pos().col;

    if ((points = updatePacLoc(oldRow + 1, oldCol)) != ILLEGAL_MV) {
      _pacman.moveDown();
      return points;
    }
  }
  return points;
}

/*
 * This function moves the pacman left on the board
 */
int Board::movePacLeft() {
  int points = 0;
  if (_pacman.get_pos().col - 1 > 0) {
    int oldRow = _pacman.get_pos().row, oldCol = _pacman.get_pos().col;

    if ((points = updatePacLoc(oldRow, oldCol - 1)) != ILLEGAL_MV) {
      _pacman.moveLeft();
      return points;
    }
  }
  return points;
}

/*
 * This function moves the pacman right on the board
 */
int Board::movePacRight() {
  int points = 0;
  if (_pacman.get_pos().col + 1 < _cols - 1) {
    int oldRow = _pacman.get_pos().row, oldCol = _pacman.get_pos().col;

    if ((points = updatePacLoc(oldRow, oldCol + 1)) != ILLEGAL_MV) {
      _pacman.moveRight();
      return points;
    }
  }
  return points;
}

/*
 * this function is called when we attempt to move the pacman on the board
 * it checks if it's legal and if after the move the pacman eats
 * a key/gift/cookie or gets eaten by a ghost and returns a value accordingly
 *
 * -2: got eaten by a ghost
 * -1: the move is illegal a wall or a door is in the way
 * 0: the move is legal but doesn't give points, moving into an empty
 *       cell or through a door while super
 * 2: eaten a cookie after the move
 * 5: ate a gift after the move
 * 7: ate a key after the move
 */
int Board::updatePacLoc(int newRow, int newCol) {
  int oldRow = _pacman.get_pos().row, oldCol = _pacman.get_pos().col;
  int points = NORMAL_MV;

  switch ((_board[newRow][newCol])) {
    case WALL:
      return ILLEGAL_MV;
      break;
    case DOOR:
      if (!_pacman.isSuper()) {
        return ILLEGAL_MV;
      } else {
        breakDoor(newRow, newCol);
        points = NORMAL_MV;
      }
      break;
    case KEY:
      points = ATE_KEY;
      keyEaten(newRow, newCol);
      break;
    case DEMON:
      if (!_pacman.isSuper()) {
        points = EATEN;
      } else {
        points = NORMAL_MV;
      }
      break;
    case GIFT:
      points = ATE_GIFT;
      giftEaten(newRow, newCol);
      break;
    case COOKIE:
      points = ATE_COOKIE;
      cookieEaten(newRow, newCol);
      break;
    default:  // SPACE
      points = NORMAL_MV;
      break;
  }

  // set the old location to an empty space
  _board[oldRow][oldCol] = SPACE;
  if (points == EATEN) {
    _board[newRow][newCol] = DEMON;
    return points;
  }

  // if the ability is active put @
  if (_pacman.isSuper()) {
    _board[newRow][newCol] = PACMAN_SUP;
    _pacman.decSuper();
  } else {
    _board[newRow][newCol] = PACMAN;
  }
  return points;
}

/*
 * this function is called when the pacman eats a key
 * it opens the first door it finds in the board
 */
void Board::keyEaten(int newRow, int newCol) {
  if (!_door_locs.empty()) {
    Location door_loc = _door_locs[0];
    _board[door_loc.row][door_loc.col] = SPACE;
    _door_locs.erase(_door_locs.begin());
  }

  auto position =
      find(_key_locs.begin(), _key_locs.end(), Location(newRow, newCol));

  if (position != _key_locs.end()) {
    _key_locs.erase(position);
  }
}

/*
 * this function is called when pacman eats a gift
 * it sets the timer to 20 moves and deletes the gift from the game
 */
void Board::giftEaten(int newRow, int newCol) {
  auto position =
      find(_gift_locs.begin(), _gift_locs.end(), Location(newRow, newCol));

  if (position != _gift_locs.end()) {
    _gift_locs.erase(position);
  }
  _pacman.setSuper();
}

/*
 * this function is called when pacman eats a cookie
 */
void Board::cookieEaten(int newRow, int newCol) {
  auto position =
      find(_cookie_locs.begin(), _cookie_locs.end(), Location(newRow, newCol));

  if (position != _cookie_locs.end()) {
    _cookie_locs.erase(position);
  }
}

/*
 * this function is called when the pacman breaks the door
 * while it has the ability active
 */
void Board::breakDoor(int newRow, int newCol) {
  auto position =
      find(_door_locs.begin(), _door_locs.end(), Location(newRow, newCol));

  if (position != _door_locs.end()) {
    _door_locs.erase(position);
  }
}

//=================== DEMON MOVERS ====================

/*
 * This function moves all the demons on the board
 * also it returns a value after all the demons have moved
 * true: pacman was eaten in the proccess
 * false: the ghost completed a round without eating the pacman
 */
bool Board::moveDemons() {
  bool makeMove, pacEaten = false;

  // loop over the demons vector and move each demon according to it's behavior
  for (auto dem = _demons.begin(); dem != _demons.end(); ++dem) {
    int oldRow = (*dem).get_pos().row, oldCol = (*dem).get_pos().col;
    makeMove = false;
    switch ((*dem).getBehavior()) {
      case HORIZANTAL:
        makeMove = moveHorizantalDemon(*dem);
        break;
      case VERTICAL:
        makeMove = moveVerticalDemon(*dem);
        break;
      case DRUNK:
        makeMove = moveDrunkDemon(*dem);
        break;
      default:
        break;
    }
    int newRow = (*dem).get_pos().row, newCol = (*dem).get_pos().col;

    if (makeMove) {
      _board[oldRow][oldCol] = SPACE;

      if (_board[newRow][newCol] == PACMAN_SUP) {
        _board[newRow][newCol] = PACMAN_SUP;
      } else if (_board[newRow][newCol] == PACMAN) {
        pacEaten = true;
      }
      _board[newRow][newCol] = DEMON;
    }
  }
  // if the demons stepped over an object the last round we rturn the object on
  // the board
  updateObjs(_cookie_locs, COOKIE);
  updateObjs(_key_locs, KEY);
  updateObjs(_gift_locs, GIFT);
  return pacEaten;
}

/*
 * This function moves the demon that moves vertically.
 * the demon moves down until it hits a wall then it walk up until a wall
 */
bool Board::moveVerticalDemon(Demon &dem) {
  int gRow = dem.get_pos().row, gCol = dem.get_pos().col;

  if (dem.isFlag()) {
    if (isMoveValid(gRow + 1, gCol)) {
      moveDemonDown(dem);
      return true;
    } else {
      dem.resetFlag();
    }

  } else {
    if (isMoveValid(gRow - 1, gCol)) {
      moveDemonUp(dem);
      return true;
    } else {
      dem.setFlag();
    }
  }
  return false;
}

/*
 * This function moves the demon that moves horizantally.
 * it moves right until it hits a wall then it move left until a wall an so on.
 */
bool Board::moveHorizantalDemon(Demon &dem) {
  int gRow = dem.get_pos().row, gCol = dem.get_pos().col;

  if (dem.isFlag()) {
    if (isMoveValid(gRow, gCol + 1)) {
      moveDemonRight(dem);
      return true;
    } else {
      dem.resetFlag();
    }

  } else {
    if (isMoveValid(gRow, gCol - 1)) {
      moveDemonLeft(dem);
      return true;
    } else {
      dem.setFlag();
    }
  }
  return false;
}

/*
 * This function moves a demon randomally.
 * it draws a random number between 0-3.
 * 0:up, 1:down, 2:right, 3: left
 */
bool Board::moveDrunkDemon(Demon &dem) {
  int gRow = dem.get_pos().row, gCol = dem.get_pos().col;
  int d = rand() % 4;

  switch (d) {
    case 0:
      if (isMoveValid(gRow - 1, gCol)) {
        moveDemonUp(dem);
        return true;
      }
      break;
    case 1:
      if (isMoveValid(gRow + 1, gCol)) {
        moveDemonDown(dem);
        return true;
      }
      break;
    case 2:
      if (isMoveValid(gRow, gCol + 1)) {
        moveDemonRight(dem);
        return true;
      }
      break;
    case 3:
      if (isMoveValid(gRow, gCol - 1)) {
        moveDemonLeft(dem);
        return true;
      }
      break;
  }
  return false;
}

/*
 * when this function is called it moves the demon up on the board
 *   if the move is legal
 */
bool Board::moveDemonUp(Demon &dem) {
  int oldRow = dem.get_pos().row, oldCol = dem.get_pos().col;
  if (isMoveValid(oldRow - 1, oldCol)) {
    dem.moveUp();
    return true;
  }
  return false;
}

/*
 * when this function is called it moves the demon down on the board
 *   if the move is legal
 */
bool Board::moveDemonDown(Demon &dem) {
  int oldRow = dem.get_pos().row, oldCol = dem.get_pos().col;
  if (isMoveValid(oldRow + 1, oldCol)) {
    dem.moveDown();
    return true;
  }
  return false;
}

/*
 * when this function is called it moves the demon left on the board
 *   if the move is legal
 */
bool Board::moveDemonLeft(Demon &dem) {
  int oldRow = dem.get_pos().row, oldCol = dem.get_pos().col;
  if (isMoveValid(oldRow, oldCol - 1)) {
    dem.moveLeft();
    return true;
  }
  return false;
}

/*
 * when this function is called it moves the demon right on the board
 *   if the move is legal
 */
bool Board::moveDemonRight(Demon &dem) {
  int oldRow = dem.get_pos().row, oldCol = dem.get_pos().col;
  if (isMoveValid(oldRow, oldCol + 1)) {
    dem.moveRight();
    return true;
  }
  return false;
}

/*
 * This function checks if the demon is not walking into a wall or door.
 * if it is it returns false, otherwise true.
 */
bool Board::isMoveValid(int newRow, int newCol) {
  return ((_board[newRow][newCol] != WALL) && (_board[newRow][newCol] != DOOR));
}

//===================== CANVAS UPDATERS ===========================

/*
 * This function prints the board on the console.
 * at first it updates the objects just incase the demons stepped over
 * keys/gifts/cookies the round before
 */
void Board::printBoard() {
  updateObjs(_cookie_locs, COOKIE);
  updateObjs(_key_locs, KEY);
  updateObjs(_gift_locs, GIFT);
  updateObjs(_door_locs, DEMON);

  for (auto g = _demons.begin(); g != _demons.end(); ++g) {
    if (_board[(*g).get_pos().row][(*g).get_pos().col] != PACMAN_SUP) {
      _board[(*g).get_pos().row][(*g).get_pos().col] = DEMON;
    }
  }

  if (_pacman.isSuper()) {
    _board[_pacman.get_pos().row][_pacman.get_pos().col] = PACMAN_SUP;
  } else {
    _board[_pacman.get_pos().row][_pacman.get_pos().col] = PACMAN;
  }

  for (int i = 0; i < _rows; i++) {
    cout << _board[i] << endl;
  }
}

/*
 * this function receives a vector of locations and a character.
 * it goes over all the location on the board and puts the given char in that
 * location
 */
void Board::updateObjs(vector<Location> locVec, char ch) {
  for (auto c = locVec.begin(); c != locVec.end(); ++c) {
    if (_board[(*c).row][(*c).col] == SPACE) {
      _board[(*c).row][(*c).col] = ch;
    }
  }
}

/*
 * this function is called when we reset the board after pacman was eaten.
 * it returns the demons and pacman to their initial location.
 */
void Board::clearObj() {
  for (auto dem = _demons.begin(); dem != _demons.end(); ++dem) {
    _board[(*dem).get_pos().row][(*dem).get_pos().col] = SPACE;
    (*dem).resetPos();
    _board[(*dem).get_pos().row][(*dem).get_pos().col] = DEMON;
  }

  _board[_pacman.get_pos().row][_pacman.get_pos().col] = SPACE;
  _pacman.resetPos();
  _board[_pacman.get_pos().row][_pacman.get_pos().col] = PACMAN;
}

/*
 * this function resets all of the board.
 * it returns the demons and pacman to their initial location
 * and updates all the other objects just incase the demons stepped over them
 */
void Board::resetBoard() {
  clearObj();
  updateObjs(_cookie_locs, COOKIE);
  updateObjs(_key_locs, KEY);
  updateObjs(_gift_locs, GIFT);
}

/*
 * This function reads the file line by line and puts it in a vector of strings
 */
void Board::ReadFile(ifstream &file) {
  string line;
  file >> _rows >> _cols;
  // build board from file
  getline(file, line);
  for (int i = 0; i < _rows; i++) {
    getline(file, line);
    _board.push_back(line);
  }
}