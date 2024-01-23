#include "Actor.h"
#include "StudentWorld.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include <queue>
#include <vector>

using namespace std;



// Actor implementations

Actor::Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* world)
	:GraphObject(imageID, startX, startY, dir, size, depth)
{
	setVisible(true);
	m_world = world;
}

Actor::~Actor() {

}

StudentWorld* Actor::getWorld() const {
	return m_world;
}

// Goodie Implementations

Goodie::Goodie(int x, int y, StudentWorld* world, Direction dir, int depth, double size, int IMAGE_ID)
	:Actor(IMAGE_ID, x, y, dir, size, depth, world)
{
	alive = true;
}

bool Goodie::checkAlive() {
	return alive;
}

void Goodie::setAlive(bool state) {
	alive = state;
}

// Tunnel Man Implementations

TunnelMan::TunnelMan(StudentWorld* world)
	:Actor(TID_PLAYER, 30, 60, right, 1.0, 0, world)
{
	hitpoints = 10;
	water = 5;
	sonar = 1;
	gold = 0;
}

TunnelMan::~TunnelMan() 
{

}

void TunnelMan::annoyed(int a) {
	hitpoints -= a;
}

void TunnelMan::doSomething() {
	int check;
	StudentWorld* world = getWorld();
	if (hitpoints <= 0) {
		return;
	}
	// USER INPUT 
	if (getWorld()->getKey(check) == true) {
		switch (check)
		{

	// MOVE LEFT
		case(KEY_PRESS_ESCAPE):
			hitpoints = 0;
			break;
		case (KEY_PRESS_LEFT):
			if (getDirection() == left) {
				if (getX() > 0 && world->overlap(world->getBoulders(), getX() - 1, getY(), 3) == false) {
					moveTo(getX() - 1, getY());
				}
			}
			else {
				setDirection(left);
			}
			break;

	// MOVE RIGHT
		case (KEY_PRESS_RIGHT):
			if (getDirection() == right && world->overlap(world->getBoulders(), getX() + 1, getY(), 3) == false) {
				if (getX() < 60) {
					moveTo(getX() + 1, getY());
				}
			}
			else {
				setDirection(right);
			}
			break;

	// MOVE UP
		case (KEY_PRESS_UP):
			if (getDirection() == up && world->overlap(world->getBoulders(), getX(), getY() + 1, 3) == false) {
				if (getY() < 60) {
					moveTo(getX(), getY() + 1);
				}
			}
			else {
				setDirection(up);
			}
			break;

	// MOVE DOWN
		case (KEY_PRESS_DOWN):
			if (getDirection() == down && world->overlap(world->getBoulders(), getX(), getY() - 1, 3) == false) {
				if (getY() > 0) {
					moveTo(getX(), getY() - 1);
				}
			}
			else {
				setDirection(down);
			}
			break;
		
	// Squirt Water
		case(KEY_PRESS_SPACE):
			if (water <= 0) {
				break;
			}
			else {
				world->playSound(SOUND_PLAYER_SQUIRT);
				if (getDirection() == left && getX() >= 4 && world->overlap(world->getBoulders(), getX() - 4, getY(), 3) == false) {
					if (world->overlapEarth(getX() - 4, 4, getY(), 4) == false) {
						world->getSquirt().push_back(new Squirt(getX() - 4, getY(), world, left));
					}
				}
				if (getDirection() == right && getX() <= 56 && world->overlap(world->getBoulders(), getX() + 4, getY(), 3) == false) {
					if (world->overlapEarth(getX() + 4, 4, getY(), 4) == false) {
						world->getSquirt().push_back(new Squirt(getX() + 4, getY(), world, right)); \
					}
				}
				if (getDirection() == up && getY() <= 56 && world->overlap(world->getBoulders(), getX(), getY() + 4, 3) == false) {
					if (world->overlapEarth(getX(), 4, getY() + 4, 4) == false) {
						world->getSquirt().push_back(new Squirt(getX(), getY() + 4, world, up));
					}
				}
				if (getDirection() == down && getY() >= 4 && world->overlap(world->getBoulders(), getX(), getY() - 4, 3) == false) {
					if (world->overlapEarth(getX(), 4, getY() - 4, 4) == false) {
						world->getSquirt().push_back(new Squirt(getX(), getY() - 4, world, down));
					}
				}
				water--;
				break;
			}
		case('z'):
			if (getSonar() <= 0) {
				break;
			}
			else {
				world->revealHidden(world->getGold(), getX(), getY(), 12);
				world->revealHidden(world->getOil(), getX(), getY(), 12);
				sonar--;
				break;
			}
		case('Z'):
			if (getSonar() <= 0) {
				break;
			}
			else {
				world->revealHidden(world->getGold(), getX(), getY(), 12);
				world->revealHidden(world->getOil(), getX(), getY(), 12);
				sonar--;
				break;
			}
		case(KEY_PRESS_TAB):
			world->getGold().push_back(new Gold(getX(), getY(), world, true, false, true));
			break;
		}
		
		
	}
	



	// DELETE OVERLAPPING EARTH

	getWorld()->deleteEarth(getX(), getY());
}

// Earth Implementations

Earth::Earth(int xpos, int ypos, StudentWorld* world)
	:Actor(TID_EARTH, xpos, ypos, right, 0.25, 3, world)
{
	isDisplayed = true;
}

Earth::~Earth() {

}

void Earth::doSomething() {

}

bool Earth::display() {
	return isDisplayed;
}

void Earth::setDisplay() {
	isDisplayed = false;
}

// Boulder Implementations

Boulder::Boulder(int x, int y, StudentWorld* world)
	:Actor(TID_BOULDER, x, y, down, 1.0, 1, world)
{
	stableState = true;
	isAlive = true;
	isWaiting = false;
	waitingTime = 0;
	isFalling = false;
}

bool Boulder::checkAlive() {
	return isAlive;
}

bool Boulder::isStable() {
	return stableState;
}

bool Boulder::getWaiting() {
	return isWaiting;
}

bool Boulder::getFalling() {
	return isFalling;
}

void Boulder::setWaiting(bool state) {
	isWaiting = state;
}

void Boulder::setStable(bool state) {
	stableState = state;
}

void Boulder::setAlive(bool alive) {
	isAlive = alive;
}

void Boulder::setFalling(bool state) {
	isFalling = state;
}

int Boulder::getCenterX() {
	return getX() + 2;
}

int Boulder::getCenterY() {
	return getY() + 2;
}


void Boulder::doSomething() {

	StudentWorld* world = getWorld();
	bool earthUnder = false;

	if (!isAlive) {
		return;
	}
	if (isStable() == true) {
		for (int i = 0; i < 4; i++) {
			if (world->getEarth()[getX() + i][getY() - 1]->display() == true) {
				earthUnder = true;
			}
		}
		if (earthUnder == false) {
			setStable(false);
			setWaiting(true);
		}
	}

	if (getWaiting() == true) {
		if (waitingTime >= 30) {
			setWaiting(false);
			setFalling(true);
			world->playSound(SOUND_FALLING_ROCK);
		}
		else {
			waitingTime++;
		}
	}

	if (getFalling() == true) {
		// Stops when boulder hits earth object
		for (int i = 0; i < 4; i++) {
			if (world->getEarth()[getX() + i][getY() - 1]->display() == true) {
				setFalling(false);
				setAlive(false);
			}
		}
		// Stop when boulder hits other boulder

		if (world->hitOtherBoulders(world->getBoulders(), getX(), getY() - 1) == true) {
			setFalling(false);
			setAlive(false);
		}
		// Moves down till the bottom
		if (getY() > 1) {
			moveTo(getX(), getY() - 1);
			if (world->overlap(world->getBoulders(), world->getTMan()->getX(), world->getTMan()->getY(), 3) == true) {
				world->getTMan()->annoyed(100);
			}
			world->annoyProtesters(world->getRegProtester(), getX(), getY(), 3, 100);
			world->annoyProtesters(world->getHardcoreProtester(), getX(), getY(), 3, 100);
		}
		else {
			setFalling(false);
			setAlive(false);
		}


	}
}

// Squirt Implementations

Squirt::Squirt(int x, int y, StudentWorld* World, Direction dir) 
	:Actor(TID_WATER_SPURT, x, y, dir, 1.0, 1, World)
{
	travelDistance = 4;
	alive = true;
}

int Squirt::getTravelDistance() {
	return travelDistance;
}

bool Squirt::checkAlive() {
	return alive;
}

void Squirt::setAlive(bool state) {
	alive = state;
}

void Squirt::decDistance() {
	travelDistance--;
}

void Squirt::doSomething() {
	StudentWorld* world = getWorld();
	if (world->annoyProtesters(world->getRegProtester(), getX(), getY(), 3, 2) == true) {
		setAlive(false);
	}
	if (world->annoyProtesters(world->getHardcoreProtester(), getX(), getY(), 3, 2) == true) {
		setAlive(false);
	}

	if (!checkAlive()) {
		return;
	}
	if (getTravelDistance() <= 0) {
		setAlive(false);
		return;
	}
	Direction dir = getDirection();

	switch (dir)
	{
	case(left):
		if (getX() - 1 < 0) {
			setAlive(false);
		}
		else if (world->overlap(world->getBoulders(), getX() - 1, getY(), 3) == true) {
			setAlive(false);
		}
		else if (getY() < 60 && world->overlapEarth(getX() - 1, 4, getY(), 4) == true) {
			setAlive(false);
		}
		else {
			moveTo(getX() - 1, getY());
			decDistance();
		}
		break;
	case(right):
		if (getX() + 4 > 63) {
			setAlive(false);
		}
		else if (world->overlap(world->getBoulders(), getX() + 1, getY(), 3) == true) {
			setAlive(false);
		}
		else if (getY() < 60 && world->overlapEarth(getX() + 1, 4, getY(), 4) == true) {
			setAlive(false);
		}
		else {
			moveTo(getX() + 1, getY());
			decDistance();
		}
		break;
	case(down):
		if (getY() - 1 < 0) {
			setAlive(false);
		}
		else if (world->overlap(world->getBoulders(), getX(), getY() - 1, 3) == true) {
			setAlive(false);
		}
		else if (getY() < 60 && world->overlapEarth(getX(), 4, getY() - 1, 4) == true) {
			setAlive(false);
		}
		else {
			moveTo(getX(), getY() - 1);
			decDistance();
		}
		break;
	case(up):
		if (getY() + 4 > 63) {
			setAlive(false);
		}
		else if (world->overlap(world->getBoulders(), getX(), getY() + 1, 3) == true) {
			setAlive(false);
		}
		else if (getY() < 60 && world->overlapEarth(getX(), 4, getY() + 1, 4) == true) {
			setAlive(false);
		}
		else {
			moveTo(getX(), getY() + 1);
			decDistance();
		}
		break;
	}


}


// Oil Implementations

Oil::Oil(int x, int y, StudentWorld* world) 
	:Goodie(x, y, world, right, 2, 1.0, TID_BARREL)
{
	setVisible(false);
}

void Oil::doSomething() {
	StudentWorld* world = getWorld();
	if (!checkAlive()) {
		return;
	}
	if (world->overlapTunnelMan(world->getTMan(), getX(), getY(), 4) == true) {
		setVisible(true);
	}
	if (world->overlapTunnelMan(world->getTMan(), getX(), getY(), 3) == true) {
		setAlive(false);
		world->playSound(SOUND_FOUND_OIL);
		world->increaseScore(1000);
		world->decOil();
	}
}

// Gold Implementations

Gold::Gold(int x, int y, StudentWorld* world, bool visibility, bool pickupTMan, bool state) 
	:Goodie(x, y, world, right, 2, 1.0, TID_GOLD)
{
	setVisible(visibility);
	TManPickup = pickupTMan;
	permenant = state;
	timeLeft = 100;
}

void Gold::doSomething() {
	StudentWorld* world = getWorld();

	if (timeLeft <= 0) {
		setAlive(false);
	}

	if (!checkAlive()) {
		return;
	}
	if (world->overlapTunnelMan(world->getTMan(), getX(), getY(), 4) == true && TManPickup == true) {
		setVisible(true);
	}
	if (world->overlapTunnelMan(world->getTMan(), getX(), getY(), 3) == true && TManPickup == true) {
		setAlive(false);
		world->playSound(SOUND_GOT_GOODIE);
		world->increaseScore(10);
		world->getTMan()->incGold();
	}

	if (TManPickup == false) {
		if (world->overlapGoldRegularProtester(world->getRegProtester(), getX(), getY(), 3, 25) == true) {
			setAlive(false);
		}
		else if (world->overlapGoldRegularProtester(world->getHardcoreProtester(), getX(), getY(), 3, 50) == true) {
			setAlive(false);
		}
		timeLeft--;
	}



}

// SONAR IMPLEMENTATIONS

Sonar::Sonar(StudentWorld* world) 
	:Goodie(0, 60, world, right, 2, 1.0, TID_SONAR)
{
	ticksRemaining = world->max(100, 300 - 10 * world->getLevel());
}

void Sonar::doSomething() {
	StudentWorld* world = getWorld();
	if (!checkAlive()) {
		return;
	}
	if (getTicks() <= 0) {
		setAlive(false);
		return;
	}
	if (world->overlapTunnelMan(world->getTMan(), getX(), getY(), 3) == true) {
		setAlive(false);
		world->playSound(SOUND_GOT_GOODIE);
		world->increaseScore(75);
		world->getTMan()->incSonar();
	}
	decTicks();
}

// WATER IMPLEMENTATIONS

Water::Water(int x, int y, StudentWorld* world) 
	:Goodie(x, y, world, right, 2, 1.0, TID_WATER_POOL)
{
	ticksRemaining = world->max(100, 300 - 10 * world->getLevel());
}

void Water::doSomething() {
	StudentWorld* world = getWorld();
	if (!checkAlive()) {
		return;
	}
	if (getTicks() <= 0) {
		setAlive(false);
		return;
	}
	if (world->overlapTunnelMan(world->getTMan(), getX(), getY(), 3) == true) {
		setAlive(false);
		world->playSound(SOUND_GOT_GOODIE);
		world->increaseScore(100);
		world->getTMan()->incWater(5);
	}
	decTicks();
}

// GENERAL PROTESTER

Protestor::Protestor(int IMAGE_ID, int hitPoints, int x, int y, StudentWorld* world) 
	:Actor(IMAGE_ID, x, y, left, 1.0, 0, world)
{
	hitpoints = hitPoints;
	numSquaresToMoveInCurrentDirection = rand() % 53 + 8;
	leaveTheOilField = false;
	ticksToWaitBetweenMoves = world->max(0, 3 - (world->getLevel() / 4));
	alive = true;
	timeToYell = 0;
	perpendicularTurn = 200;
}

// Check if there is straight path between the proteter and the tunnelman
bool Protestor::straightPath() {
	StudentWorld* world = getWorld();
	if (getX() == world->getTMan()->getX() || getY() == world->getTMan()->getY()) {
		if (getX() == world->getTMan()->getX()) { // Same X coords and protestor is higher than the tunnelMan
			if (getY() > world->getTMan()->getY()) {
				for (int i = getY(); i >= world->getTMan()->getY(); i--) {
					if (world->overlapEarth(getX(), 4, i, 4) == true || world->overlap(world->getBoulders(), getX(), i, 3) == true) {
						return false;
					}
				}
				return true;
			}
			else if (getY() < world->getTMan()->getY()) {  // Same X coords and protestor is lower than the tunnelMan
				for (int i = getY(); i <= world->getTMan()->getY(); i++) {
					if (world->overlapEarth(getX(), 4, i, 4) == true || world->overlap(world->getBoulders(), getX(), i, 3) == true) {
						return false;
					}
				}
				return true;
			}
		}
		if (getY() == world->getTMan()->getY()) { // Same Y coords and protestor to the right of the tunnelMan
			if (getX() > world->getTMan()->getX()) {
				for (int i = getX(); i >= world->getTMan()->getX(); i--) {
					if (world->overlapEarth(i, 4, getY(), 4) == true || world->overlap(world->getBoulders(), i, getY(), 3) == true) {
						return false;
					}
				}
				return true;
			}
			else if (getX() < world->getTMan()->getX()) { // Same Y coords and protestor to the left of the tunnelMan
				for (int i = getX(); i <= world->getTMan()->getX(); i++) {
					if (world->overlapEarth(i, 4, getY(), 4) == true || world->overlap(world->getBoulders(), i, getY(), 3) == true) {
						return false;
					}
				}
				return true;
			}
		}
	}
	return false;
}






// Regular Protester

RegularProtester::RegularProtester(int x, int y, StudentWorld* world) 
	:Protestor(TID_PROTESTER, 5, x, y, world)
{
	timeDead = 0;
}

void RegularProtester::bribed() {
	getWorld()->findPath(getX(), getY(), 60, 60, points);
	setOilFieldState(true);
	setWaitingTicks(0);
}

void RegularProtester::annoyed(int a) {

	if (getHitpoints() > 0) {
		decHitpoints(a);
		getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
		if (getWaitingTicks() <= getWorld()->max(0, 3 - (getWorld()->getLevel() / 4))) {
			incWaitingTicks(getWorld()->max(50, 100 - getWorld()->getLevel() * 10));
		}
	}
	if (getHitpoints() <= 0 && getOilFieldState() == false) {
		getWorld()->findPath(getX(), getY(), 60, 60, points);
		setOilFieldState(true);
		getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
		setWaitingTicks(0);
		if (getWorld()->overlap(getWorld()->getBoulders(), getX(), getY(), 3) == true) {
			getWorld()->increaseScore(500);
		}
		else {
			getWorld()->increaseScore(100);
		}
	}
}

void RegularProtester::doSomething() {
	StudentWorld* world = getWorld();
	if (!checkAlive()) {
		return;
	}
	if (getWaitingTicks() > 0) {
		decWaitingTicks(1);
		return;
	}
	// Check if the protester is leaving the field
	if (getOilFieldState() == true) {
		if (getX() == 60 && getY() == 60) {
			setAlive(false);
		}
		else {
			int xPos = points.back()->x;
			int yPos = points.back()->y;
			points.pop_back();

			if (xPos > getX()) { // Move right
				if (getDirection() == right) {
					moveTo(xPos, yPos);
				}
				else {
					setDirection(right);
				}
			}

			else if (xPos < getX()) { // Move left
				if (getDirection() == left) {
					moveTo(xPos, yPos);
				}
				else {
					setDirection(left);
				}
			}
			else if (yPos > getY()) { // Move up
				if (getDirection() == up) {
					moveTo(xPos, yPos);
				}
				else {
					setDirection(up);
				}
			}
			else { // Mode down
				if (getDirection() == down) {
					moveTo(xPos, yPos);
				}
				else {
					setDirection(down);
				}
			}
			setWaitingTicks(world->max(0, 3 - (world->getLevel() / 4)));
		}
		return;
	}
	if (world->overlapTunnelMan(world->getTMan(), getX(), getY(), 4) == true) {
		if (getYell() <= 0) {
			world->playSound(SOUND_PROTESTER_YELL);
			world->getTMan()->annoyed(2);
			setYell(15);
			setWaitingTicks(world->max(0, 3 - (world->getLevel() / 4)));
			decPerpendicularTurn();
			return;
		}
		else {
			setWaitingTicks(world->max(0, 3 - (world->getLevel() / 4)));
			decTimeToYell(1);
			decPerpendicularTurn();
			return;
		}
	}
	// Checking if protester can see the Tunnelman
	if (straightPath() == true) {
		if (getX() == world->getTMan()->getX()) { // Same X coords and protestor is higher than the tunnelMan
			if (getY() > world->getTMan()->getY()) {
				setDirection(down);
				moveTo(getX(), getY() - 1);
			}
			if (getY() < world->getTMan()->getY()) { // Same X coords and protestor is lower than the tunnelMan
				setDirection(up);
				moveTo(getX(), getY() + 1);
			}
		}
		else if (getY() == world->getTMan()->getY()) { // Same Y coords and protestor to the right of the tunnelMan
			if (getX() > world->getTMan()->getX()) {
				setDirection(left);
				moveTo(getX() - 1, getY());
			}
			if (getX() < world->getTMan()->getX()) {// Same Y coords and protestor to the left of the tunnelMan
				setDirection(right);
				moveTo(getX() + 1, getY());
			}
		}
		setNumSquare(0); // num squares to move is set to 0.
		setWaitingTicks(world->max(0, 3 - (world->getLevel() / 4)));
		decTimeToYell(1);
		decPerpendicularTurn();
		return;
	}

	// Protestor can't see the tunnelman

	// Pick a new direction if num steps is over

	if (getSquare() <= 0) {
		for (;;) {
			int dir = rand() % 4;
			if (dir == 0) { // Left
				if (getX() - 1 > 0 && world->overlapEarth(getX() - 1, 4, getY(), 4) == false && world->overlap(world->getBoulders(), getX() - 1, getY(), 3) == false) {
					setDirection(left);
					break;
				}
			}
			if (dir == 1) { // Right
				if (getX() + 1 <= 60 && world->overlapEarth(getX() + 1, 4, getY(), 4) == false && world->overlap(world->getBoulders(), getX() + 1, getY(), 3) == false) {
					setDirection(right);
					break;
				}
			}
			if (dir == 2) { // Down
				if (getY() - 1 > 0 && world->overlapEarth(getX(), 4, getY() - 1, 4) == false && world->overlap(world->getBoulders(), getX(), getY() - 1, 3) == false) {
					setDirection(right);
					break;
				}
			}
			if (dir == 3) { // Up
				if (getY() + 1 <= 60 && world->overlapEarth(getX(), 4, getY() + 1, 4) == false && world->overlap(world->getBoulders(), getX(), getY() + 1, 3) == false) {
					setDirection(up);
					break;
				}
			}
		}
		setRandomNumSquare();
	}

	// Move in intersections

	else if (getPerpendicularTurn() <= 0) {
		if (getDirection() == left || getDirection() == right) {
			if (getY() - 1 > 0 && world->overlapEarth(getX(), 4, getY() - 1, 4) == false && world->overlap(world->getBoulders(), getX(), getY() - 1, 3) == false && // down and up
				getY() + 1 <= 60 && world->overlapEarth(getX(), 4, getY() + 1, 4) == false && world->overlap(world->getBoulders(), getX(), getY() + 1, 3) == false) { 
				int chooseDir = rand() % 2;
				if (chooseDir == 1) {
					setDirection(down);
					setRandomNumSquare();
					setWaitingTicks(world->max(0, 3 - (world->getLevel() / 4)));
					decTimeToYell(1);
					setPerpendicularTurn(200);
				}
				else {
					setDirection(up);
					setRandomNumSquare();
					setWaitingTicks(world->max(0, 3 - (world->getLevel() / 4)));
					decTimeToYell(1);
					setPerpendicularTurn(200);
				}
			}
			else if (getY() - 1 > 0 && world->overlapEarth(getX(), 4, getY() - 1, 4) == false && world->overlap(world->getBoulders(), getX(), getY() - 1, 3) == false) {
				setDirection(down);
				setRandomNumSquare();
				setWaitingTicks(world->max(0, 3 - (world->getLevel() / 4)));
				decTimeToYell(1);
				setPerpendicularTurn(200);
			}
			else if (getY() + 1 <= 60 && world->overlapEarth(getX(), 4, getY() + 1, 4) == false && world->overlap(world->getBoulders(), getX(), getY() + 1, 3) == false) {
				setDirection(up);
				setRandomNumSquare();
				setWaitingTicks(world->max(0, 3 - (world->getLevel() / 4)));
				decTimeToYell(1);
				setPerpendicularTurn(200);
			}
		}
		else if (getDirection() == up || getDirection() == down) {
			if (getX() - 1 > 0 && world->overlapEarth(getX() - 1, 4, getY(), 4) == false && world->overlap(world->getBoulders(), getX() - 1, getY(), 3) == false && // left and right turns
				getX() + 1 <= 60 && world->overlapEarth(getX() + 1, 4, getY(), 4) == false && world->overlap(world->getBoulders(), getX() + 1, getY(), 3) == false) {
				int chooseDir = rand() % 2;
				if (chooseDir == 1) {
					setDirection(left);
					setRandomNumSquare();
					setWaitingTicks(world->max(0, 3 - (world->getLevel() / 4)));
					decTimeToYell(1);
					setPerpendicularTurn(200);
				}
				else {
					setDirection(right);
					setRandomNumSquare();
					setWaitingTicks(world->max(0, 3 - (world->getLevel() / 4)));
					decTimeToYell(1);
					setPerpendicularTurn(200);
				}
			}
			else if (getX() - 1 > 0 && world->overlapEarth(getX() - 1, 4, getY(), 4) == false && world->overlap(world->getBoulders(), getX() - 1, getY(), 3) == false) {
				setDirection(left);
				setRandomNumSquare();
				setWaitingTicks(world->max(0, 3 - (world->getLevel() / 4)));
				decTimeToYell(1);
				setPerpendicularTurn(200);
			}
			else if (getX() + 1 <= 60 && world->overlapEarth(getX() + 1, 4, getY(), 4) == false && world->overlap(world->getBoulders(), getX() + 1, getY(), 3) == false) {
				setDirection(right);
				setRandomNumSquare();
				setWaitingTicks(world->max(0, 3 - (world->getLevel() / 4)));
				decTimeToYell(1);
				setPerpendicularTurn(200);
			}
		}
	}

	// Attempt to move in current direction

	Direction dir = getDirection();

	switch(dir) {
		case(left):
			if (getX() - 1 > 0 && world->overlapEarth(getX() - 1, 4, getY(), 4) == false && world->overlap(world->getBoulders(), getX() - 1, getY(), 3) == false) {
				moveTo(getX() - 1, getY());
				decNumSquare(1); 
				break;
			}
			else {
				setNumSquare(0);
				break;
			}
		case(right):
			if (getX() + 1 <= 60 && world->overlapEarth(getX() + 1, 4, getY(), 4) == false && world->overlap(world->getBoulders(), getX() + 1, getY(), 3) == false) {
				moveTo(getX() + 1, getY());
				decNumSquare(1);
				break;
			}
			else {
				setNumSquare(0);
				break;
			}
		case(down):
			if (getY() - 1 > 0 && world->overlapEarth(getX(), 4, getY() - 1, 4) == false && world->overlap(world->getBoulders(), getX(), getY() - 1, 3) == false) {
				moveTo(getX(), getY() - 1);
				decNumSquare(1);
				break;
			}
			else {
				setNumSquare(0);
				break;
			}
		case(up):
			if (getY() + 1 <= 60 && world->overlapEarth(getX(), 4, getY() + 1, 4) == false && world->overlap(world->getBoulders(), getX(), getY() + 1, 3) == false) {
				moveTo(getX(), getY() + 1);
				decNumSquare(1);
				break;
			}
			else {
				setNumSquare(0);
				break;
			}
	}
	setWaitingTicks(world->max(0, 3 - (world->getLevel() / 4)));
	decTimeToYell(1);
	decPerpendicularTurn();
}

// Hardcore Protester Implementations

HardcoreProtester::HardcoreProtester(int x, int y, StudentWorld* world)
	:Protestor(TID_HARD_CORE_PROTESTER, 20, x, y, world)
{
	numMovesToTunnelMan = 16 + (world->getLevel()) * 2;
}

void HardcoreProtester::bribed() {
	incWaitingTicks(getWorld()->max(50, 100 - getWorld()->getLevel() * 10));
}

void HardcoreProtester::annoyed(int a) {

	if (getHitpoints() > 0) {
		decHitpoints(a);
		getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
		if (getWaitingTicks() <= getWorld()->max(0, 3 - (getWorld()->getLevel() / 4))) {
			incWaitingTicks(getWorld()->max(50, 100 - getWorld()->getLevel() * 10));
		}
	}
	if (getHitpoints() <= 0 && getOilFieldState() == false) {
		if (getWorld()->overlap(getWorld()->getBoulders(), getX(), getY(), 3) == true) {
			getWorld()->increaseScore(500);
		}
		else {
			getWorld()->increaseScore(250);
		}
		getWorld()->findPath(getX(), getY(), 60, 60, points);
		setOilFieldState(true);
		getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
		setWaitingTicks(0);
	}
}

void HardcoreProtester::doSomething() {
	StudentWorld* world = getWorld();
	int waitingTIme = world->max(0, 3 - (world->getLevel() / 4));

	if (!checkAlive()) {
		return;
	}
	if (getWaitingTicks() > 0) {
		decWaitingTicks(1);
		return;
	}
	// Check if the protester is leaving the field
	if (getOilFieldState() == true) {
		if (getX() == 60 && getY() == 60) {
			setAlive(false);
		}
		else {
			int xPos = points.back()->x;
			int yPos = points.back()->y;
			points.pop_back();

			if (xPos > getX()) { // Move right
				if (getDirection() == right) {
					moveTo(xPos, yPos);
				}
				else {
					setDirection(right);
				}
			}

			else if (xPos < getX()) { // Move left
				if (getDirection() == left) {
					moveTo(xPos, yPos);
				}
				else {
					setDirection(left);
				}
			}
			else if (yPos > getY()) { // Move up
				if (getDirection() == up) {
					moveTo(xPos, yPos);
				}
				else {
					setDirection(up);
				}
			}
			else { // Mode down
				if (getDirection() == down) {
					moveTo(xPos, yPos);
				}
				else {
					setDirection(down);
				}
			}
			setWaitingTicks(world->max(0, 3 - (world->getLevel() / 4)));
		}
		return;
	}
	if (world->overlapTunnelMan(world->getTMan(), getX(), getY(), 4) == true) {
		if (getYell() <= 0) {
			world->playSound(SOUND_PROTESTER_YELL);
			world->getTMan()->annoyed(2);
			setYell(15);
			setWaitingTicks(world->max(0, 3 - (world->getLevel() / 4)));
			decPerpendicularTurn();
			return;
		}
		else {
			setWaitingTicks(world->max(0, 3 - (world->getLevel() / 4)));
			decTimeToYell(1);
			decPerpendicularTurn();
			return;
		}
	}

	// Hone down on TunnelMan if possible
	vector<coordinates*> map;
	if (world->findPath(getX(), getY(), world->getTMan()->getX(), world->getTMan()->getY(), map) == true) {
		int xPos = map.back()->x;
		int yPos = map.back()->y;
		map.pop_back();
		if (map.size() <= numMovesToTunnelMan) {
			if (xPos > getX()) { // Move right
				if (getDirection() == right) {
					moveTo(xPos, yPos);
				}
				else {
					setDirection(right);
					moveTo(xPos, yPos);
				}
			}

			else if (xPos < getX()) { // Move left
				if (getDirection() == left) {
					moveTo(xPos, yPos);
				}
				else {
					setDirection(left);
					moveTo(xPos, yPos);
				}
			}
			else if (yPos > getY()) { // Move up
				if (getDirection() == up) {
					moveTo(xPos, yPos);
				}
				else {
					setDirection(up);
					moveTo(xPos, yPos);
				}
			}
			else { // Mode down
				if (getDirection() == down) {
					moveTo(xPos, yPos);
				}
				else {
					setDirection(down);
					moveTo(xPos, yPos);
				}
			}
			setWaitingTicks(world->max(0, 3 - (world->getLevel() / 4)));
			decTimeToYell(1);
			return;
		}
	}


	if (straightPath() == true) {
		if (getX() == world->getTMan()->getX()) { // Same X coords and protestor is higher than the tunnelMan
			if (getY() > world->getTMan()->getY()) {
				setDirection(down);
				moveTo(getX(), getY() - 1);
			}
			if (getY() < world->getTMan()->getY()) { // Same X coords and protestor is lower than the tunnelMan
				setDirection(up);
				moveTo(getX(), getY() + 1);
			}
		}
		else if (getY() == world->getTMan()->getY()) { // Same Y coords and protestor to the right of the tunnelMan
			if (getX() > world->getTMan()->getX()) {
				setDirection(left);
				moveTo(getX() - 1, getY());
			}
			if (getX() < world->getTMan()->getX()) {// Same Y coords and protestor to the left of the tunnelMan
				setDirection(right);
				moveTo(getX() + 1, getY());
			}
		}
		setNumSquare(0); // num squares to move is set to 0.
		setWaitingTicks(world->max(0, 3 - (world->getLevel() / 4)));
		decTimeToYell(1);
		decPerpendicularTurn();
		return;
	}


	// Pick a new direction if num steps is over

	if (getSquare() <= 0) {
		for (;;) {
			int dir = rand() % 4;
			if (dir == 0) { // Left
				if (getX() - 1 > 0 && world->overlapEarth(getX() - 1, 4, getY(), 4) == false && world->overlap(world->getBoulders(), getX() - 1, getY(), 3) == false) {
					setDirection(left);
					break;
				}
			}
			if (dir == 1) { // Right
				if (getX() + 1 <= 60 && world->overlapEarth(getX() + 1, 4, getY(), 4) == false && world->overlap(world->getBoulders(), getX() + 1, getY(), 3) == false) {
					setDirection(right);
					break;
				}
			}
			if (dir == 2) { // Down
				if (getY() - 1 > 0 && world->overlapEarth(getX(), 4, getY() - 1, 4) == false && world->overlap(world->getBoulders(), getX(), getY() - 1, 3) == false) {
					setDirection(right);
					break;
				}
			}
			if (dir == 3) { // Up
				if (getY() + 1 <= 60 && world->overlapEarth(getX(), 4, getY() + 1, 4) == false && world->overlap(world->getBoulders(), getX(), getY() + 1, 3) == false) {
					setDirection(up);
					break;
				}
			}
		}
		setRandomNumSquare();
	}

	// Move in intersections

	else if (getPerpendicularTurn() <= 0) {
		if (getDirection() == left || getDirection() == right) {
			if (getY() - 1 > 0 && world->overlapEarth(getX(), 4, getY() - 1, 4) == false && world->overlap(world->getBoulders(), getX(), getY() - 1, 3) == false && // down and up
				getY() + 1 <= 60 && world->overlapEarth(getX(), 4, getY() + 1, 4) == false && world->overlap(world->getBoulders(), getX(), getY() + 1, 3) == false) {
				int chooseDir = rand() % 2;
				if (chooseDir == 1) {
					setDirection(down);
					setRandomNumSquare();
					setWaitingTicks(world->max(0, 3 - (world->getLevel() / 4)));
					decTimeToYell(1);
					setPerpendicularTurn(200);
				}
				else {
					setDirection(up);
					setRandomNumSquare();
					setWaitingTicks(world->max(0, 3 - (world->getLevel() / 4)));
					decTimeToYell(1);
					setPerpendicularTurn(200);
				}
			}
			else if (getY() - 1 > 0 && world->overlapEarth(getX(), 4, getY() - 1, 4) == false && world->overlap(world->getBoulders(), getX(), getY() - 1, 3) == false) {
				setDirection(down);
				setRandomNumSquare();
				setWaitingTicks(world->max(0, 3 - (world->getLevel() / 4)));
				decTimeToYell(1);
				setPerpendicularTurn(200);
			}
			else if (getY() + 1 <= 60 && world->overlapEarth(getX(), 4, getY() + 1, 4) == false && world->overlap(world->getBoulders(), getX(), getY() + 1, 3) == false) {
				setDirection(up);
				setRandomNumSquare();
				setWaitingTicks(world->max(0, 3 - (world->getLevel() / 4)));
				decTimeToYell(1);
				setPerpendicularTurn(200);
			}
		}
		else if (getDirection() == up || getDirection() == down) {
			if (getX() - 1 > 0 && world->overlapEarth(getX() - 1, 4, getY(), 4) == false && world->overlap(world->getBoulders(), getX() - 1, getY(), 3) == false && // left and right turns
				getX() + 1 <= 60 && world->overlapEarth(getX() + 1, 4, getY(), 4) == false && world->overlap(world->getBoulders(), getX() + 1, getY(), 3) == false) {
				int chooseDir = rand() % 2;
				if (chooseDir == 1) {
					setDirection(left);
					setRandomNumSquare();
					setWaitingTicks(world->max(0, 3 - (world->getLevel() / 4)));
					decTimeToYell(1);
					setPerpendicularTurn(200);
				}
				else {
					setDirection(right);
					setRandomNumSquare();
					setWaitingTicks(world->max(0, 3 - (world->getLevel() / 4)));
					decTimeToYell(1);
					setPerpendicularTurn(200);
				}
			}
			else if (getX() - 1 > 0 && world->overlapEarth(getX() - 1, 4, getY(), 4) == false && world->overlap(world->getBoulders(), getX() - 1, getY(), 3) == false) {
				setDirection(left);
				setRandomNumSquare();
				setWaitingTicks(world->max(0, 3 - (world->getLevel() / 4)));
				decTimeToYell(1);
				setPerpendicularTurn(200);
			}
			else if (getX() + 1 <= 60 && world->overlapEarth(getX() + 1, 4, getY(), 4) == false && world->overlap(world->getBoulders(), getX() + 1, getY(), 3) == false) {
				setDirection(right);
				setRandomNumSquare();
				setWaitingTicks(world->max(0, 3 - (world->getLevel() / 4)));
				decTimeToYell(1);
				setPerpendicularTurn(200);
			}
		}
	}

	// Attempt to move in current direction

	Direction dir = getDirection();

	switch (dir) {
	case(left):
		if (getX() - 1 > 0 && world->overlapEarth(getX() - 1, 4, getY(), 4) == false && world->overlap(world->getBoulders(), getX() - 1, getY(), 3) == false) {
			moveTo(getX() - 1, getY());
			decNumSquare(1);
			break;
		}
		else {
			setNumSquare(0);
			break;
		}
	case(right):
		if (getX() + 1 <= 60 && world->overlapEarth(getX() + 1, 4, getY(), 4) == false && world->overlap(world->getBoulders(), getX() + 1, getY(), 3) == false) {
			moveTo(getX() + 1, getY());
			decNumSquare(1);
			break;
		}
		else {
			setNumSquare(0);
			break;
		}
	case(down):
		if (getY() - 1 > 0 && world->overlapEarth(getX(), 4, getY() - 1, 4) == false && world->overlap(world->getBoulders(), getX(), getY() - 1, 3) == false) {
			moveTo(getX(), getY() - 1);
			decNumSquare(1);
			break;
		}
		else {
			setNumSquare(0);
			break;
		}
	case(up):
		if (getY() + 1 <= 60 && world->overlapEarth(getX(), 4, getY() + 1, 4) == false && world->overlap(world->getBoulders(), getX(), getY() + 1, 3) == false) {
			moveTo(getX(), getY() + 1);
			decNumSquare(1);
			break;
		}
		else {
			setNumSquare(0);
			break;
		}
	}
	setWaitingTicks(world->max(0, 3 - (world->getLevel() / 4)));
	decTimeToYell(1);
	decPerpendicularTurn();
}



// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
