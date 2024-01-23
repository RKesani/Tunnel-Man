#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include <string>
#include <vector>
#include <queue>

class Actor;
class TunnelMan;
class Earth;
class Boulder;
class Squirt;
class Oil;
class Gold;
class Sonar;
class Water;
class RegularProtester;
struct coordinates;
class HardcoreProtester;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);

	virtual ~StudentWorld();

	virtual int init();

	virtual int move();

	virtual void cleanUp();

	void deleteEarth(int x, int y);

	void addBoulders();

	void addOil();

	void addGold();

	void addWaterPool();

	bool viableMovement(int x, int y);

	bool findPath(int startX, int startY, int endX, int endY, std::vector<coordinates*> &points);


	std::string displayText();

	// General function to check ditance from other objects in the field also sotred ina  vector
	template <typename Data>
	bool actorDistance(int x, int y, std::vector<Data*> actors) {
		for (auto actor : actors) {
			if ((actor->getX() - x) * (actor->getX() - x) + (actor->getY() - y) * (actor->getY() - y) <= 36) {
				return false;
			}
		}
		return true;
	}

	// Delete vector objects - general
	template <typename Data>
	void deleteActor(std::vector<Data*> &actors) {
		while (!actors.empty()) {
			delete actors.back();
			actors.pop_back();
		}
	}

	// Delete Dead vector objects - general
	template <typename Data>
	void deleteDeadActor(std::vector<Data*> &actors) {
		for (int i = actors.size() - 1; i >= 0; i--) {
			if (actors[i]->checkAlive() == false) {
				delete actors[i];
				actors.erase(actors.begin() + i);
			}
		}
	}

	
	bool overlapEarth(int colStart, int cols, int rowStart, int rows);

	template <typename Data>
	bool overlap(std::vector<Data*> actors, int x, int y, int radius) {
		for (auto actor : actors) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					for (int k = 3 - radius; k < radius + 1; k++) {
						for (int l = 3 - radius; l < radius + 1; l++) {
							if (actor->getX() + k == x + i && actor->getY() + l == y + j) {
								return true;
							}
						}
					}
				}
			}
		}
		return false;
	}

	template <typename Data>
	bool overlapBouldersStable(std::vector<Data*> actors, int x, int y, int radius) {
		for (auto actor : actors) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					for (int k = 3 - radius; k < radius + 1; k++) {
						for (int l = 3 - radius; l < radius + 1; l++) {
							if (actor->getX() + k == x + i && actor->getY() + l == y + j && actor->isStable() == true) {
								return true;
							}
						}
					}
				}
			}
		}
		return false;
	}


	template <typename Data>
	bool overlapGoldRegularProtester (std::vector<Data*> actors, int x, int y, int radius, int value) {
		for (auto actor : actors) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					for (int k = 3 - radius; k < radius + 1; k++) {
						for (int l = 3 - radius; l < radius + 1; l++) {
							if (actor->getX() + k == x + i && actor->getY() + l == y + j) {
								playSound(SOUND_PROTESTER_FOUND_GOLD);
								increaseScore(value);
								actor->bribed();
								return true;
							}
						}
					}
				}
			}
		}
		return false;
	}

	template <typename Data>
	bool annoyProtesters(std::vector<Data*> actors, int x, int y, int radius, int annoy) {
		for (auto actor : actors) {
			bool annoyed = false;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					for (int k = 3 - radius; k < radius + 1; k++) {
						for (int l = 3 - radius; l < radius + 1; l++) {
							if (actor->getX() + k == x + i && actor->getY() + l == y + j) {
								annoyed = true;
							}
						}
					}
				}
			}
			if (annoyed == true) {
				actor->annoyed(annoy);
				return true;
			}
		}
		return false;
	}

	template <typename Data>
	void revealHidden(std::vector<Data*> actors, int x, int y, int radius) {
		for (auto actor : actors) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					for (int k = 3 - radius; k < radius + 1; k++) {
						for (int l = 3 - radius; l < radius + 1; l++) {
							if (actor->getX() + k == x + i && actor->getY() + l == y + j) {
								actor->setVisible(true);
							}
						}
					}
				}
			}
		}
	}


	template <typename Data>
	bool overlapTunnelMan(Data* actors, int x, int y, int radius) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				for (int k = 3 - radius; k < radius + 1; k++) {
					for (int l = 3 - radius; l < radius + 1; l++) {
						if (actors->getX() + k == x + i && actors->getY() + l == y + j) {
							return true;
						}
					}
				}
			}
		}
		return false;
	}

	bool hitOtherBoulders(std::vector<Boulder*>, int x, int y);

	// Get list of Boulders
	std::vector<Boulder*> getBoulders() {
		return boulders;
	}

	// Get the tunnelMan pointer
	TunnelMan* getTMan() {
		return protagonist;
	}

	// Get the pointer to Earth variables
	typedef Earth* (*array_pointers)[60];
	array_pointers getEarth() {
		return earthObjects;
	}

	// Get pointer to squirt variables

	std::vector<Squirt*>& getSquirt() {
		return squirts;
	}

	// Get Pointer to oil barrels

	std::vector<Oil*>& getOil() {
		return oilBarrels;
	}

	// Get pointer to gold pieces

	std::vector<Gold*>& getGold() {
		return goldPieces;
	}

	// GEt a pointer to sonar kits

	std::vector<Sonar*>& getSonar() {
		return sonarKits;
	}

	// Get a pointer to water pools

	std::vector<Water*>& getWater() {
		return waterPools;
	}

	// Get a pointer to reg protesters

	std::vector<RegularProtester*>& getRegProtester() {
		return regProtesters;
	}

	std::vector<HardcoreProtester*>& getHardcoreProtester() {
		return hardcoreProtesters;
	}


	int min(int a, int b);

	int max(int a, int b);

	

	void decOil() {
		oilLeft--;
	}

	int howMuchOilLeft() {
		return oilLeft;
	}


private:
	TunnelMan* protagonist; // tunnelman
	Earth* earthObjects[64][60]; // All the Earth objects
	std::vector<Boulder*> boulders;
	std::vector<Squirt*> squirts;
	std::vector<Oil*> oilBarrels;
	std::vector<Gold*> goldPieces;
	std::vector<Sonar*> sonarKits;
	std::vector<Water*> waterPools;
	std::vector<RegularProtester*> regProtesters;
	std::vector<HardcoreProtester*> hardcoreProtesters;
	int oilLeft;
	int goodieChance;
	int ticksSinceLastProtester;
	int targetProtesters;
	int numProtesters;

};

#endif // STUDENTWORLD_H_
