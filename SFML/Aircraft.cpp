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
#include "Aircraft.h"
#include "DataTables.h"
#include "TextNode.h"
#include <string>
#include "Utility.h"

namespace GEX {
	namespace
	{
		//const is all uppercase and this is for all the aircraft data (it had to be initialized when declared because its const
		const std::map<AircraftType, AircraftData> TABLE = initializeAircraftData();
	}

	GEX::Aircraft::Aircraft(AircraftType type, const TextureManager & textures)
		: Entity(TABLE.at(type).hitpoints),//set the hitpoints depending on the type of plane
		_type(type),
		_sprite(textures.get(TABLE.at(type).texture))
	{
		sf::FloatRect bounds = _sprite.getLocalBounds();
		_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

		std::unique_ptr<TextNode> health(new TextNode(""));
		_healthDisplay = health.get();
		attachChild(std::move(health));

		updateTexts();
	}

	void GEX::Aircraft::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{
		target.draw(_sprite, states);
	}
	unsigned int Aircraft::getCategory() const
	{
		switch (_type)
		{
		case GEX::AircraftType::Eagle:
			return Category::PlayerAircraft;
			break;
		case GEX::AircraftType::Raptor:
			Category::AlliedAircraft;
			break;
		default:
			return Category::EnemyAircraft;
			break;
		}
	}
	void Aircraft::updateTexts()
	{
		_healthDisplay->setString(std::to_string(getHitPoints()) + " HP");
		_healthDisplay->setPosition(0.f, 50.f);
		_healthDisplay->setRotation(-getRotation());
	}
	void Aircraft::updateCurrent(sf::Time dt)
	{
		updateMovementPattern(dt);
		Entity::updateCurrent(dt);
		updateTexts();
	}
	void Aircraft::updateMovementPattern(sf::Time dt)
	{
		// movement pattern of the auto moving planes
		const std::vector<Direction>& directions = TABLE.at(_type).directions;
		
		//if there are directions modify movement
		if (!directions.empty())
		{
			// if you moved for the length of the direction distance
			if (_travelDistance > (directions[_directionIndex].distance))
			{
				//add one but mod the size so it doesnt go out of bounds and reset travel distance
				_directionIndex = (++_directionIndex) % directions.size();
				_travelDistance = 0.f;
			}

			float radians = toRadian(directions[_directionIndex].angle + 90.f);
			float vx = getMaxSpeed() * std::cos(radians);
			float vy = getMaxSpeed() * std::sin(radians);

			setVelocity(vx, vy);

			_travelDistance += getMaxSpeed() * dt.asSeconds();
		}
	}
	float Aircraft::getMaxSpeed() const
	{
		return TABLE.at(_type).speed;
	}
}