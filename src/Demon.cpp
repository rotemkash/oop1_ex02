#include "Demon.h"

/* Constructor */
Demon::Demon(Location loc, Behavior behave, bool flag)
    : _pos(loc), _init_pos(loc), _behave(behave), _flag(flag) {}

/* sets position of the demon */
void Demon::set_pos(Location new_pos) { _pos = new_pos; }

/* retuns the location of the demon */
Location Demon::get_pos() { return _pos; }

/* resets the location of the demon to it's initial location*/
void Demon::resetPos() { _pos = _init_pos; }

/* moves the demon one step up */
void Demon::moveUp() { _pos.row--; }

/* moves the demon one step down */
void Demon::moveDown() { _pos.row++; }

/* moves the demon one step down */
void Demon::moveLeft() { _pos.col--; }

/* moves the demon one step down */
void Demon::moveRight() { _pos.col++; }

/* returns the behavior of the demon*/
Behavior Demon::getBehavior() { return _behave; }

/* returns the movement flag:
 * this flag is used for demons who move vertically/horizantally
 * as long this flag it true the demon moves one direction until it hits a wall
 * then the flag is set to false and now the demon move the other direction
 * until it hits a wall and so on.
 */
bool Demon::isFlag() { return _flag; }

/* sets the flag to true*/
void Demon::setFlag() { _flag = true; }

/* resets the flag to false*/
void Demon::resetFlag() { _flag = false; }