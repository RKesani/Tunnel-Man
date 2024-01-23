#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameWorld.h"
#include "StudentWorld.h"
#include "vector"
#include <cstdlib>


class StudentWorld;
struct coordinates {
	coordinates(int xpos, int ypos, int previousXpos, int previousYpos) {
		x = xpos;
		y = ypos;
		previousX = previousXpos;
		previousY = previousYpos;
	}
	int x;
	int y;
	int previousX;
	int previousY;
};



class Actor: public GraphObject {
public:
	Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* world); // constructor
	virtual void doSomething() = 0; // Actions for the objects
	virtual ~Actor(); //Destructor
	virtual void annoyed(int a) = 0;
	StudentWorld* getWorld() const;
private:
	StudentWorld* m_world;
};

class TunnelMan: public Actor {
public:
	TunnelMan(StudentWorld* world);
	virtual void doSomething();
	virtual ~TunnelMan();
	virtual void annoyed(int a);

	// Accessors
	int getHitpoints() {
		return hitpoints;
	}
	int getWater(){
		return water;
	}
	int getSonar() {
		return sonar;
	}
	int getGold() {
		return gold;
	}
	// Mutators
	void incGold() {
		gold++;
	}
	void incSonar() {
		sonar++;
	}
	void incWater(int a) {
		water += a;
	}

private:
	int hitpoints;
	int water;
	int sonar;
	int gold;
};

class Earth: public Actor
{
public:
	Earth(int xpos, int ypos, StudentWorld* world);
	virtual void doSomething();
	virtual ~Earth();
	virtual void annoyed(int a) {} // does nothing in Earth object
	bool display();
	void setDisplay();
private:
	bool isDisplayed;
};

class Boulder : public Actor {
public:
	Boulder(int x, int y, StudentWorld* world);
	virtual ~Boulder() {}
	virtual void annoyed(int a) {} // does nothing in Boulder object
	// Mutators
	void setStable(bool state);
	void setAlive(bool alive);
	void setWaiting(bool state);
	void setFalling(bool state);
	virtual void doSomething();

	// Accessors
	bool isStable();
	bool checkAlive();
	int getCenterX();
	int getCenterY();
	bool getWaiting();
	bool getFalling();


private:
	bool stableState;
	bool isAlive;
	bool isWaiting;
	int waitingTime;
	bool isFalling;
	
};

class Squirt : public Actor {
public:
	Squirt(int x, int y, StudentWorld* World, Direction dir);
	virtual ~Squirt() {};
	virtual void annoyed(int a) {} // does nothing in Boulder object
	virtual void doSomething();

	// Mutators
	void decDistance();
	void setAlive(bool state);
	
	// Accesors
	int getTravelDistance();
	bool checkAlive();
private:
	int travelDistance;
	bool alive;
};

class Goodie : public Actor {
public:
	Goodie(int x, int y, StudentWorld* world, Direction dir, int depth, double size, int IMAGE_ID);
	virtual ~Goodie() {};
	virtual void annoyed(int a) = 0; // Goodies will never be called and is an ADT
	virtual void doSomething() = 0;

	// Accessors
	bool checkAlive();

	//Mutators
	void setAlive(bool state);

private:
	bool alive;

};

class Oil : public Goodie {
public:
	Oil(int x, int y, StudentWorld* world);
	virtual ~Oil() {};
	virtual void annoyed(int a) {}; // Oil can't get annoyed
	virtual void doSomething();
};

class Gold : public Goodie {
public: 
	Gold(int x, int y, StudentWorld* world, bool visibility, bool pickupTMan, bool state);
	virtual ~Gold() {};
	virtual void annoyed(int a) {} // Gold can't be annoyed
	virtual void doSomething();

	// Accessors
	bool getPickup() {
		return TManPickup;
	}

	bool getPermenant() {
		return permenant;
	}

	// Mutators
	void setPickup(bool state) {
		TManPickup = state;
	}

	void setPermenant(bool state) {
		permenant = state;
	}
private:
	bool TManPickup;
	bool permenant;
	int timeLeft;
};

class Sonar : public Goodie {
public:
	Sonar(StudentWorld* world);
	virtual ~Sonar() {};
	virtual void annoyed(int a) {}; // Sonar kits can't be annoyed!!
	virtual void doSomething();

	// Accessors
	int getTicks() {
		return ticksRemaining;
	}
	//Mutators
	void decTicks() {
		ticksRemaining--;
	}

private:
	int ticksRemaining;
};

class Water : public Goodie {
public:
	Water(int x, int y, StudentWorld* world);
	virtual ~Water() {};
	virtual void annoyed(int a) {}; // Water ca't be annoyed duh
	virtual void doSomething();
	// Accessors
	int getTicks() {
		return ticksRemaining;
	}
	//Mutators
	void decTicks() {
		ticksRemaining--;
	}

private:
	int ticksRemaining;
};

class Protestor : public Actor {
public:
	Protestor(int IMAGE_ID, int hitPoints, int x, int y, StudentWorld* world);
	virtual ~Protestor() {};
	virtual void annoyed(int a) = 0;

	virtual void doSomething() = 0;
	bool straightPath();

	// Accessors
	int getHitpoints() {
		return hitpoints;
	}
	int getSquare() {
		return numSquaresToMoveInCurrentDirection;
	}
	bool getOilFieldState() {
		return leaveTheOilField;
	}

	int getWaitingTicks() {
		return ticksToWaitBetweenMoves;
	}

	bool checkAlive() {
		return alive;
	}
	
	int getYell() {
		return timeToYell;
	}

	int getPerpendicularTurn() {
		return perpendicularTurn;
	}



	//Mutators

	void setNumSquare(int a) {
		numSquaresToMoveInCurrentDirection = a;
	}

	void setRandomNumSquare() {
		numSquaresToMoveInCurrentDirection = rand() % 53 + 8;
	}

	void decNumSquare(int a) {
		numSquaresToMoveInCurrentDirection -= a;
	}

	void setOilFieldState(bool state) {
		leaveTheOilField = state;
	}

	void decWaitingTicks(int a) {
		ticksToWaitBetweenMoves -= a;
	}

	void setWaitingTicks(int a) {
		ticksToWaitBetweenMoves = a;
	}

	void incWaitingTicks(int a) {
		ticksToWaitBetweenMoves += a;
	}

	void setAlive(bool state) {
		alive = state;
	}

	void setYell(int a) {
		timeToYell = a;
	}

	void decTimeToYell(int a) {
		timeToYell -= a;
	}

	void decPerpendicularTurn() {
		perpendicularTurn--;
	}

	void setPerpendicularTurn(int a) {
		perpendicularTurn = a;
	}

	void decHitpoints(int a) {
		hitpoints -= a;
	}

private:
	int hitpoints;
	int numSquaresToMoveInCurrentDirection;
	bool leaveTheOilField;
	int ticksToWaitBetweenMoves;
	bool alive;
	int timeToYell;
	int perpendicularTurn;

};

class RegularProtester : public Protestor {
public:
	RegularProtester(int x, int y, StudentWorld* world);
	virtual ~RegularProtester() {}
	virtual void annoyed(int a);
	virtual void doSomething();
	void bribed();

private:
	std::vector<coordinates*> points;
	int timeDead;
};

class HardcoreProtester : public Protestor {
public:
	HardcoreProtester(int x, int y, StudentWorld* world);
	virtual ~HardcoreProtester() {}
	virtual void annoyed(int a);
	virtual void doSomething();
	void bribed();

private:
	std::vector<coordinates*> points;
	std::vector<coordinates*> mapToTunnelman;
	int numMovesToTunnelMan;
};
#endif // ACTOR_H