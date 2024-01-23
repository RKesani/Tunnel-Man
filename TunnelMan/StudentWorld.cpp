#include "StudentWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "Actor.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <queue>
using namespace std;

class Boulder;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(string assetDir) 
	: GameWorld(assetDir)
{
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 60; j++) {

			earthObjects[i][j] = nullptr;
		}
	}

	protagonist = nullptr;
}

StudentWorld::~StudentWorld() {
	delete protagonist;
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 60; j++) {

			delete earthObjects[i][j];
		}
	}
}

int StudentWorld::init() {

	// initial variables
	goodieChance = (getLevel() * 25 + 300);
	ticksSinceLastProtester = 0;
	targetProtesters = min(15, 2 + getLevel() * 1.5);
	numProtesters = 0;

	// Create TunnelMan
	protagonist = new TunnelMan(this);


	// Create Earth Objects

	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 60; j++) {

			// Don't insert Earth Objects in the middle shaft
			if (i >= 30 && i <= 33 && j >= 4) {
				continue;
			}

			// Allocating new Earth Objects
			earthObjects[i][j] = new Earth(i, j, this);
		}
	}

	// Add Boulders
	for (int i = 0; i < min(getLevel()/2 + 2, 9); i++) {
		addBoulders();
	}
	// Add Oil
	for (int i = 0; i < min(2 + getLevel(), 21); i++) {
		addOil();
		oilLeft++;
	}
	// Add Gold
	for (int i = 0; i < max(5 - getLevel() / 2, 2); i++) {
		addGold();
	}

	

	return GWSTATUS_CONTINUE_GAME;
}

bool StudentWorld::overlapEarth(int colStart, int cols, int rowStart, int rows) {
	for (int i = colStart; i < colStart + (cols); i++) {
		for (int j = rowStart; j < rowStart + (rows); j++) {
			if (i < 64 && j < 60 && earthObjects[i][j] != nullptr && earthObjects[i][j]->display() == true) {
				return true;
			}
		}
	}
	return false;
}

int StudentWorld::min(int a, int b) {
	if (a < b) {
		return a;
	}
	else {
		return b;
	}
}

int StudentWorld::max(int a, int b){
	if (a > b) {
		return a;
	}
	else {
		return b;
	}
}
void StudentWorld::addBoulders() {
	for (;;) {
		int x = rand() % 61;
		int y = rand() % 37 + 20;

		if (x >= 26 && x <= 34 && y >= 1) {
			continue;
		}

		if (actorDistance(x, y, boulders) == false) {
			continue;
		}

		if (actorDistance(x, y, oilBarrels) == false) {
			continue;
		}

		if (actorDistance(x, y, goldPieces) == false) {
			continue;
		}

		boulders.push_back(new Boulder(x, y, this));
		deleteEarth(x, y);
		break;
	}
}

void StudentWorld::addOil() {
	for (;;) {
		int x = rand() % 61;
		int y = rand() % 57;

		if (x >= 26 && x <= 34 && y >= 1) {
			continue;
		}

		if (actorDistance(x, y, boulders) == false) {
			continue;
		}

		if (actorDistance(x, y, oilBarrels) == false) {
			continue;
		}

		if (actorDistance(x, y, goldPieces) == false) {
			continue;
		}

		oilBarrels.push_back(new Oil(x, y, this));
		break;
	}
}

void StudentWorld::addGold() {
	for (;;) {
		int x = rand() % 61;
		int y = rand() % 57;

		if (x >= 26 && x <= 34 && y >= 1) {
			continue;
		}

		if (actorDistance(x, y, boulders) == false) {
			continue;
		}

		if (actorDistance(x, y, oilBarrels) == false) {
			continue;
		}

		if (actorDistance(x, y, goldPieces) == false) {
			continue;
		}

		goldPieces.push_back(new Gold(x, y, this, false, true, true));
		break;
	}
}

void StudentWorld::addWaterPool() {
	for (;;) {
		int x = rand() % 64;
		int y = rand() % 60;

		if (overlapEarth(x, 4, y, 4) == true) {
			continue;
		}
		else {
			waterPools.push_back(new Water(x, y, this));
			break;
		}
	}
}

string StudentWorld::displayText() {
	ostringstream oss;
	oss << "Lvl: " << setw(2) << getLevel(); // Level
	oss << "  Lives: " << getLives(); // Lives;
	oss << "  Hlth " << setw(3) << (getTMan()->getHitpoints()) * 10 << "%"; // Hlth %
	oss << "  Wtr " << setw(2) << getTMan()->getWater(); // Water
	oss << "  Gld " << setw(2) << getTMan()->getGold(); // Gold;
	oss << "  Oil Left: " << setw(2) << oilLeft; // Oil
	oss << "  Sonar: " << setw(2) << getTMan()->getSonar(); // Sonar
	oss.fill('0');
	oss << "  Scr: " << setw(6) << getScore(); // Score
	return oss.str();
}


int StudentWorld::move() {
	setGameStatText(displayText());
	ticksSinceLastProtester--;
	// Add protesters
	if (ticksSinceLastProtester <= 0 && hardcoreProtesters.size() + regProtesters.size() < targetProtesters) {
		int probabilityOfHardcore = min(90, getLevel() * 10 + 30);

		int randnum =  rand() % 100 + 1;

		if (randnum <= probabilityOfHardcore) {
			hardcoreProtesters.push_back(new HardcoreProtester(60, 60, this));
		}
		else {
			regProtesters.push_back(new RegularProtester(60, 60, this));
		}
		ticksSinceLastProtester = max(25, 200 - getLevel());
	}

	protagonist->doSomething();

	for (auto squirt : squirts) {
		squirt->doSomething();
	}

	for (auto boulder : boulders) {
			boulder->doSomething();
		if (boulder->checkAlive() == false) {
			boulder->setVisible(false);
		}
	}

	for (auto oil : oilBarrels) {
		oil->doSomething();
	}

	for (auto gold : goldPieces) {
		gold->doSomething();
	}

	for (auto sonar : sonarKits) {
		sonar->doSomething();
	}

	for (auto water : waterPools) {
		water->doSomething();
	}

	for (auto protester : regProtesters) {
		protester->doSomething();
	}

	for (auto protester : hardcoreProtesters) {
		protester->doSomething();
	}

	// Add goodies
	int addGoodie = rand() % goodieChance;
	if (addGoodie == 1) {
		int waterOrSonar = rand() % 5;
		if (waterOrSonar == 1) {
			sonarKits.push_back(new Sonar(this));
		}
		else {
			addWaterPool();
		}
	}

	deleteDeadActor(sonarKits);
	deleteDeadActor(boulders);
	deleteDeadActor(squirts);
	deleteDeadActor(oilBarrels);
	deleteDeadActor(goldPieces);
	deleteDeadActor(waterPools);
	deleteDeadActor(regProtesters);
	deleteDeadActor(hardcoreProtesters);
	

	if (oilLeft == 0) {
		return GWSTATUS_FINISHED_LEVEL;
	}

	if (protagonist->getHitpoints() <= 0) {
		playSound(SOUND_PLAYER_GIVE_UP);
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
	delete protagonist;
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 60; j++) {

			delete earthObjects[i][j];
		}
	}
	deleteActor(boulders);
	deleteActor(squirts);
	deleteActor(oilBarrels);
	deleteActor(goldPieces);
	deleteActor(sonarKits);
	deleteActor(waterPools);
	deleteActor(regProtesters);
	deleteActor(hardcoreProtesters);

	oilLeft = 0;
}

void StudentWorld::deleteEarth(int x, int y) {
	bool sound = false;
	for (int i = x; i < x + 4; i++) {
		for (int j = y; j < y + 4; j++) {
			if (i < 64 && j < 60 && earthObjects[i][j] != nullptr) {
				if (earthObjects[i][j]->display() == true) {
					earthObjects[i][j]->setVisible(false);
					earthObjects[i][j]->setDisplay();
					sound = true;
				}
			}
		}
	}	
	if (sound == true) {
		playSound(SOUND_DIG);
	}
}

bool StudentWorld::hitOtherBoulders(vector<Boulder*> boulder_list, int x, int y) {
	for (auto boulder : boulder_list) {
		for (int i = x; i < x + 4; i++) {
			for (int j = boulder->getX(); j < boulder->getX() + 4; j++) {
				if (i == j && y == (boulder->getY() + 3) && boulder->isStable() == true) {
					return true;
				}
			}
		}
	}
	return false;
}

bool StudentWorld::viableMovement(int x, int y) {
	if (x <= 60 && x >= 0 && y <= 60 && y >= 0 && overlapEarth(x, 4, y, 4) == false && overlapBouldersStable(boulders, x, y, 3) == false) {
		return true;
	}
	else {
		return false;
	}
}

bool StudentWorld::findPath(int startX, int startY, int endX, int endY, vector<coordinates*> &points) {
	queue<coordinates> search;
	vector<coordinates> visited;
	bool visitedArray[61][61] = { false };
	coordinates temp(startX, startY, startX, startY);
	search.push(temp); // Starting point has the same previous points
	
	while (!search.empty()) {
		temp = search.front();
		int i = temp.x;
		int j = temp.y;
		visited.push_back(temp);
		search.pop();

		if (i == endX && j == endY) {
			int xP = temp.previousX;
			int yP = temp.previousY;
			points.push_back(new coordinates(i, j, temp.previousX, temp.previousY));
			for (int i = visited.size() - 2; i > 0; i--) {
				if (visited[i].x == xP && visited[i].y == yP) {
					xP = visited[i].previousX;
					yP = visited[i].previousY;
					points.push_back(new coordinates(visited[i].x, visited[i].y, visited[i].previousX, visited[i].previousY));
				 }
			}
			return true;
		}
		else {
			// Check left
			if (viableMovement(i - 1, j) && visitedArray[i-1][j] == false) {
				coordinates newcord(i - 1, j, i, j);
				search.push(newcord);
				visitedArray[i - 1][j] = true;
			}
			// Check Right
			if (viableMovement(i + 1, j) && visitedArray[i + 1][j] == false) {
				coordinates newcord(i + 1, j, i, j);
				search.push(newcord);
				visitedArray[i + 1][j] = true;
			}
			// Check up
			if (viableMovement(i, j + 1) && visitedArray[i][j + 1] == false) {
				coordinates newcord(i, j + 1, i, j);
				search.push(newcord);
				visitedArray[i][j + 1] = true;
			}
			// Check down
			if (viableMovement(i, j - 1) && visitedArray[i][j - 1] == false) {
				coordinates newcord(i, j - 1, i, j);
				search.push(newcord);
				visitedArray[i][j - 1] = true;
			}
		}
	}
	return false;
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
