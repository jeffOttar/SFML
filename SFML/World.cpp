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


namespace GEX {
	World::World(sf::RenderWindow& window) :
	_window(window),
	_worldView(window.getDefaultView()),
	_textures(),
	_sceneGraph(),
	_sceneLayers(),
	_worldBounds(0.f,0.f, _worldView.getSize().x, 5000.f),//left top width height
	_spawnPosition(_worldView.getSize().x / 2.f,_worldBounds.height - _worldView.getSize().y / 2.f),
	_scrollSpeed(-50.f),
	_playerAircraft(nullptr)
	{
		loadTextures();
		buildScene();

		//prepare the view
		_worldView.setCenter(_spawnPosition);
	}

	void World::update(sf::Time dt)
	{
		//scroll the world
		_worldView.move(0,_scrollSpeed*dt.asSeconds());//move on the y at scroll speed
		
		_playerAircraft->setVelocity(0.f, _scrollSpeed);

		//while the commandQueue is not empty send the next command to the scene graph
		while (!_commandQueue.isEmpty())
		{
			_sceneGraph.onCommand(_commandQueue.pop(), dt);
		}

		adaptPlayerVelocity();
		_sceneGraph.update(dt);
		adaptPlayerPosition();
		
	}

	void World::adaptPlayerVelocity()
	{
		sf::Vector2f velocity = _playerAircraft->getVelocity();

		if(velocity.x != 0.f && velocity.y != 0.f)
		{
			_playerAircraft->setVelocity(velocity / std::sqrt(2.f));
		}
	}

	void World::adaptPlayerPosition() 
	{
		const float BORDER_DISTANCE=40.f;

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
	}

	void World::buildScene()
	{
		//initialize layers
		for (int i = 0; i < LayerCount; i++) {
			SceneNode::Ptr layer(new SceneNode());
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

		//add escort planes
		std::unique_ptr<Aircraft> leftEscort(new Aircraft(AircraftType::Avenger, _textures));
		leftEscort->setPosition(-80.f, 50.f);
		_playerAircraft->attachChild(std::move(leftEscort));

		std::unique_ptr<Aircraft> rightEscort(new Aircraft(AircraftType::Raptor, _textures));
		rightEscort->setPosition(80.f, 50.f);
		_playerAircraft->attachChild(std::move(rightEscort));

		//add enemy planes
		std::unique_ptr<Aircraft> enemy(new Aircraft(AircraftType::Avenger, _textures));
		enemy->setPosition(_spawnPosition.x + 100.f, _spawnPosition.y - 600.f);
		enemy->setVelocity(0.f, -_scrollSpeed);//speed is scrollspeed going down
		enemy->setRotation(180);//rotate them the opposite way
		_sceneLayers[Air]->attachChild(std::move(enemy));

		enemy = std::unique_ptr<Aircraft>(new Aircraft(AircraftType::Raptor, _textures));
		enemy->setPosition(_spawnPosition.x - 100.f, _spawnPosition.y - 600.f);
		enemy->setVelocity(50.f, -_scrollSpeed);//speed is scrollspeed going down
		enemy->setRotation(180);//rotate them the opposite way
		_sceneLayers[Air]->attachChild(std::move(enemy));

		enemy = std::unique_ptr<Aircraft>(new Aircraft(AircraftType::Eagle, _textures));
		enemy->setPosition(_spawnPosition.x - 70.f, _spawnPosition.y - 600.f);
		enemy->setVelocity(50.f, -_scrollSpeed);//speed is scrollspeed going down
		enemy->setRotation(180);//rotate them the opposite way
		_sceneLayers[Air]->attachChild(std::move(enemy));

	}



}