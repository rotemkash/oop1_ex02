
#include "PacMan.h"

/* Constructor */
PacMan::PacMan() : _pos(0, 0), _init_pos(0, 0), _is_super(0) {}

/* Sets position of pacman to new_pos*/
void PacMan::set_pos(Location new_pos) { _pos = new_pos; }

/* returns the location of Pacman */
Location PacMan::get_pos() { return _pos; }

/* Sets the initial postiotion of Pacman*/
void PacMan::set_init_pos(Location pos) { _init_pos = pos; }

/* returns the initial location of pacman*/
Location PacMan::get_init_pos() { return _init_pos; }

/* returns pacman to it's initial ocation */
void PacMan::resetPos() { _pos = _init_pos; }

/* moves Pacman one step up */
void PacMan::moveUp() { _pos.row--; }

/* moves Pacman one step down */
void PacMan::moveDown() { _pos.row++; }

/* moves Pacman one step left */
void PacMan::moveLeft() { _pos.col--; }

/* moves Pacman one step right */
void PacMan::moveRight() { _pos.col++; }

/* returns true if the super Ability is active, false otherwise*/
bool PacMan::isSuper() { return (_is_super > 0); }

/* starts the timer for the super ability */
void PacMan::setSuper() { _is_super = SUPER_TIME; }

/* decreases the timer for the super ability*/
void PacMan::decSuper() { _is_super--; }