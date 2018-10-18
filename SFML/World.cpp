/**
* @file
* @author Jeff Ottar-
* @version 1.0
*
*
* @section DESCRIPTION
* <  >
*
*
* @section LICENSE
*
*
* Copyright 2017
* Permission to use, copy, modify, and/or distribute this software for
* any purpose with or without fee is hereby granted, provided that the
* above copyright notice and this permission notice appear in all copies.
*
* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
* WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
* ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
* WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
* ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*
* @section Academic Integrity
* I certify that this work is solely my own and complies with
* NBCC Academic Integrity Policy (policy 1111)
*/
#include "World.h"
#include "Pickup.h"
#include "Projectile.h"

#include <cmath>
#include <algorithm>



namespace GEX {
	World::World(sf::RenderWindow& window) :
		_window(window),
		_worldView(window.getDefaultView()),
		_textures(),
		_sceneGraph(),
		_sceneLayers(),
		_worldBounds(0.f, 0.f, _worldView.getSize().x, 5000.f),//left top width height
		_spawnPosition(_worldView.getSize().x / 2.f, _worldBounds.height - _worldView.getSize().y / 2.f),
		_scrollSpeed(-50.f),
		_playerAircraft(nullptr),
		_enemySpawnPoints(),
		_activeEnemies()
	{
		loadTextures();
		buildScene();

		//prepare the view
		_worldView.setCenter(_spawnPosition);
	}

	void World::update(sf::Time dt, CommandQueue& commands)
	{
		//scroll the world
		_worldView.move(0, _scrollSpeed*dt.asSeconds());//move on the y at scroll speed

		_playerAircraft->setVelocity(0.f, _scrollSpeed);

		//guideMissiles();

		//while the commandQueue is not empty send the next command to the scene graph
		while (!_commandQueue.isEmpty())
		{
			_sceneGraph.onCommand(_commandQueue.pop(), dt);
		}

		adaptPlayerVelocity();

		handleCollisions();

		_sceneGraph.update(dt, commands);
		adaptPlayerPosition();

		spawnEnemies();
	}

	void World::adaptPlayerVelocity()
	{
		sf::Vector2f velocity = _playerAircraft->getVelocity();

		if (velocity.x != 0.f && velocity.y != 0.f)
		{
			_playerAircraft->setVelocity(velocity / std::sqrt(2.f));
		}
	}

	void World::addEnemies()
	{
		addEnemy(AircraftType::Raptor, 0.f, 200.f);
		addEnemy(AircraftType::Raptor, 250.f, 200.f);
		addEnemy(AircraftType::Raptor, -250.f, 200.f);
		
		addEnemy(AircraftType::Avenger, -70.f, 400.f);//these start at end of screen
		addEnemy(AircraftType::Avenger, 70.f, 400.f);

		//addEnemy(AircraftType::Avenger, -170.f, 600.f);
		//addEnemy(AircraftType::Avenger, 170.f, 600.f);

		addEnemy(AircraftType::Avenger, -140.f, 600.f);
		addEnemy(AircraftType::Avenger, 140.f, 600.f);

		//sort the planes based on spawn point location 
		std::sort(_enemySpawnPoints.begin(), _enemySpawnPoints.end(),
			[](SpawnPoint lhs, SpawnPoint rhs)
		{
			return lhs.y < rhs.y;
		});
	}

	void World::addEnemy(AircraftType type, float relX, float relY)
	{
		//relY and relX stand for relative x and y
		SpawnPoint spawnPoint(type, _spawnPosition.x + relX, _spawnPosition.y - relY);
		_enemySpawnPoints.push_back(spawnPoint);
	}

	void World::spawnEnemies()
	{
		//while there are enemies to spawn and the enemy is located within the battlefield
		while (!_enemySpawnPoints.empty() && _enemySpawnPoints.back().y > getBattlefieldBounds().top)
		{
			auto spawnPoint = _enemySpawnPoints.back();

			std::unique_ptr<Aircraft> enemy(new Aircraft(spawnPoint.type, _textures));
			enemy->setPosition(spawnPoint.x, spawnPoint.y);
			enemy->setRotation(180.f);
			//move the enemy to the scenegraph
			_sceneLayers[Air]->attachChild(std::move(enemy));
			//remove the spawned enemy from the list
			_enemySpawnPoints.pop_back();
		}
	}

	sf::FloatRect World::getViewBounds() const
	{
		return sf::FloatRect(_worldView.getCenter() - _worldView.getSize() / 2.f, _worldView.getSize());
	}

	sf::FloatRect World::getBattlefieldBounds() const
	{
		//get the view bounds and expand the height by 100 pixels and reduce the top by 100 pixels(+y goes down)
		sf::FloatRect bounds = getViewBounds();
		bounds.top -= 100.f;
		bounds.height += 100.f;
		return bounds;
	}

	void World::guideMissiles()
	{
		//build list of active enemies
		Command enemyCollector;
		enemyCollector.category = Category::Type::EnemyAircraft;//everything with category enemy with execute this command
		enemyCollector.action = derivedAction<Aircraft>([this](Aircraft& enemy, sf::Time dt) 
		{
			if (!enemy.isDestroyed())//dont add destroyed enemies to list
			{
				_activeEnemies.push_back(&enemy);
			}
		});

		Command missileGuider;
		missileGuider.category = Category::Type::AlliedProjectile;//the derived action is used for safe downcasts
		missileGuider.action = derivedAction<Projectile>([this](Projectile& missile, sf::Time dt)
		{
			//ignore bullets only guide missiles
			if (!missile.isGuided())
			{
				return;//if it is a bullet return
			}

			float minDistance = std::numeric_limits<float>::max();
			//start the min distance as the max float and when you find a smaller value set that as minimum distance
			Aircraft* closestEnemy = nullptr;

			for (Aircraft* e : _activeEnemies)
			{
				auto d = distance(missile, *e);
				if (d < minDistance)
				{
					minDistance = d;
					closestEnemy = e;
				}
			}
			if (closestEnemy)
			{
				missile.guidedTowards(closestEnemy->getWorldPosition());
			}
		});

		_commandQueue.push(enemyCollector);
		_commandQueue.push(missileGuider);
		_activeEnemies.clear();
	}

	/**
	*check if they have matching categories or if they match but are in the wrong order
	*
	*/
	bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
	{
		unsigned int category1 = colliders.first->getCategory();
		unsigned int category2 = colliders.second->getCategory();

		// check if they match categories
		if (type1 & category1 && type2 & category2)
		{
			return true;
		}
		else if (type1 & category2 && type2 & category1)//check if they match but in the wrong order
		{
			std::swap(colliders.first, colliders.second);//swap them the right way
			return true;
		}
		else
		{
			return false;
		}
	}

	void World::handleCollisions()
	{
		//build list of colliding pairs
		std::set<SceneNode::Pair> collisionPairs;
		_sceneGraph.checkSceneCollision(_sceneGraph, collisionPairs);


		for (SceneNode::Pair pair : collisionPairs)
		{
			//check for matching categories for the player and enemy two enemies colliding does nothing
			if (matchesCategories(pair, Category::Type::PlayerAircraft, Category::Type::EnemyAircraft))
			{
				//if there is a player and enemy colliding then cast them into variables
				auto& player = static_cast<Aircraft&>(*(pair.first));
				auto& enemy = static_cast<Aircraft&>(*(pair.second));

				//when a collsion occurs the player loses health amounting to enemy health
				player.damage(enemy.getHitPoints());
				enemy.destroy();
			}
			else if (matchesCategories(pair, Category::Type::PlayerAircraft, Category::Type::Pickup))
			{
				auto& player = static_cast<Aircraft&>(*pair.first);
				auto& pickup = static_cast<Pickup&>(*pair.second);

				// Apply pickup effect to player, destroy projectile
				pickup.apply(player);
				pickup.destroy();
			}
			else if ((matchesCategories(pair, Category::Type::EnemyAircraft, Category::Type::AlliedProjectile))
				|| (matchesCategories(pair, Category::Type::PlayerAircraft, Category::Type::EnemyProjectile)))
			{
				auto& aircraft = static_cast<Aircraft&>(*pair.first);
				auto& projectile = static_cast<Projectile&>(*pair.second);

				//projectile damages aircraft and then is destroyed
				aircraft.damage(projectile.getDamage());
				projectile.destroy();
			}
		}
	}


	void World::adaptPlayerPosition()
	{
		const float BORDER_DISTANCE = 40.f;

		sf::FloatRect viewBounds(_worldView.getCenter() - _worldView.getSize() / 2.f, //get the 0,0 of the bounds by getting center and then half of the height and width
			_worldView.getSize());
		sf::Vector2f position = _playerAircraft->getPosition();

		//adjust position so it stays within the left and right bounds
		position.x = std::max(position.x, viewBounds.left + BORDER_DISTANCE);
		position.x = std::min(position.x, viewBounds.left + viewBounds.width - BORDER_DISTANCE);

		//adjust position so it stays within the bounds
		position.y = std::max(position.y, viewBounds.top + BORDER_DISTANCE);
		position.y = std::min(position.y, viewBounds.top + viewBounds.height - BORDER_DISTANCE);

		_playerAircraft->setPosition(position);
	}

	void World::draw()
	{
		_window.setView(_worldView);
		_window.draw(_sceneGraph);
	}

	CommandQueue & World::getCommandQueue()
	{
		return _commandQueue;
	}

	void World::loadTextures()
	{
		_textures.load(GEX::TextureID::Eagle, "Media/Media/Textures/Eagle.png");
		_textures.load(GEX::TextureID::Raptor, "Media/Media/Textures/Raptor.png");
		_textures.load(GEX::TextureID::Avenger, "Media/Media/Textures/Avenger.png");
		_textures.load(GEX::TextureID::Desert, "Media/Media/Textures/Desert.png");
		_textures.load(GEX::TextureID::Bullet, "Media/Media/Textures/Bullet.png");
		_textures.load(GEX::TextureID::Missile, "Media/Media/Textures/Missile.png");
		_textures.load(GEX::TextureID::FireRate, "Media/Media/Textures/FireRate.png");
		_textures.load(GEX::TextureID::FireSpread, "Media/Media/Textures/FireSpread.png");
		_textures.load(GEX::TextureID::MissileRefill, "Media/Media/Textures/MissileRefill.png");
		_textures.load(GEX::TextureID::HealthRefill, "Media/Media/Textures/HealthRefill.png");
	}

	void World::buildScene()
	{
		//initialize layers
		for (int i = 0; i < LayerCount; i++) {
			Category::Type category = (i == Air) ? Category::Type::AirSceneLayer : Category::Type::None;
			SceneNode::Ptr layer(new SceneNode(category));
			
			//SceneNode::Ptr layer(new SceneNode());
			_sceneLayers.push_back(layer.get());
			_sceneGraph.attachChild(std::move(layer));
		}

		//draw background
		sf::Texture& texture = _textures.get(TextureID::Desert);//get reference to texture
		sf::IntRect textureRect(_worldBounds);
		texture.setRepeated(true);//repeat the texture

		//add sprite
		std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));//create sprite node using the texture and bounds
		backgroundSprite->setPosition(_worldBounds.left, _worldBounds.top);//make texture bounds line up with world bounds
		_sceneLayers[Background]->attachChild(std::move(backgroundSprite));//

		//add player aircraft and game objects
		std::unique_ptr<Aircraft> leader(new Aircraft(AircraftType::Eagle, _textures));
		leader->setPosition(_spawnPosition);
		leader->setVelocity(0.f, _scrollSpeed);
		_playerAircraft = leader.get();
		_sceneLayers[Air]->attachChild(std::move(leader));

		addEnemies();

	}



}