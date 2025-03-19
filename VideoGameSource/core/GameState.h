#pragma once

#include <vector>
#include <SFML/Graphics/View.hpp>
#include <unordered_map>
#include <core/ViewLayers.h>
namespace sf
{
	class Time;
	class RenderWindow;

}
class Projectile;
class StandardEnemy;

class GameState
{

	std::unordered_map<ViewLayers, sf::View> stateViews;


	virtual int BoundaryCheckBullets() {}
	virtual int BoundaryCheckEnemies() {}
	virtual int BoundaryCheckPickups() {}
	virtual int BoundaryCheckLoot() {}
	virtual int playerCheckBullets() {}
	virtual int enemyCheckBullets() {}
	virtual int playerCheckEnemies() {}

public:
	virtual ~GameState() = 0;

	virtual int Update(sf::Time& dt_) = 0; 
	virtual int Render(sf::RenderWindow& wnd_) = 0;
	virtual int Input(sf::Time& dt_) = 0;
	virtual int ReadjustCollided(sf::Time& dt_) {};
	virtual int HandleCollisions(sf::Time& dt_) {};
	virtual int AnimateScene(sf::Time& dt_) {}

};