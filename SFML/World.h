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
#pragma once
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "SceneNode.h"
#include "SpriteNode.h"
#include "TextureManager.h"
#include "Aircraft.h"
#include "CommandQueue.h"

#include <memory>
#include <iostream>

namespace sf
{
	class RenderWindow;
}
namespace GEX {


	class World
	{
	public:
		World(sf::RenderWindow& window);
		void								update(sf::Time dt,CommandQueue& commands);
		void								draw();

		CommandQueue&						getCommandQueue();

	private:
		void								loadTextures();
		void								buildScene();
		void								adaptPlayerPosition();
		void								adaptPlayerVelocity();

		void								addEnemies();//add enemies spawn points for them to spawn at
		void								addEnemy(AircraftType type, float relX, float relY);
		void								spawnEnemies();

		sf::FloatRect						getViewBounds() const;
		sf::FloatRect						getBattlefieldBounds() const;

		void								guideMissiles();
		void								handleCollisions();

	private: 
		enum Layer
		{
			Background = 0,
			Air,
			LayerCount
		};

		struct SpawnPoint
		{
			SpawnPoint(AircraftType _type, float _x, float _y) :
				type(_type),
				x(_x),
				y(_y)
			{
			}

			AircraftType type;
			float x;
			float y;
		};


	private:
		sf::RenderWindow&					_window;//the order that they are declared in the .h is the order they are created in the initialization list
		sf::View                            _worldView;
		TextureManager                      _textures;

		SceneNode                           _sceneGraph;
		std::vector<SceneNode*>				_sceneLayers;

		CommandQueue						_commandQueue;
		sf::FloatRect                       _worldBounds;
		sf::Vector2f                        _spawnPosition;
		float                               _scrollSpeed;
		Aircraft*                           _playerAircraft;

		std::vector<SpawnPoint>				_enemySpawnPoints;

		std::vector<Aircraft*>				_activeEnemies;
	};

}