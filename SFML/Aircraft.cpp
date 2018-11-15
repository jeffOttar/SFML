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
#include <memory>
#include "Utility.h"
#include "CommandQueue.h"
#include "Category.h"
#include "SoundNode.h"
#include <functional>

using namespace std::placeholders;

namespace GEX {
	namespace
	{
		//const is all uppercase and this is for all the aircraft data (it had to be initialized when declared because its const
		const std::map<AircraftType, AircraftData> TABLE = initializeAircraftData();
	}

	GEX::Aircraft::Aircraft(AircraftType type, const TextureManager & textures)
		: Entity(TABLE.at(type).hitpoints),//set the hitpoints depending on the type of plane
		_type(type),
		_sprite(textures.get(TABLE.at(type).texture), TABLE.at(type).textureRect),
		_explosion(textures.get(TextureID::Explosion)),
		_showExplosion(true),
		_healthDisplay(nullptr),
		_missileDisplay(nullptr),
		_travelDistance(0.f),
		_directionIndex(0),
		_isFiring(false),
		_hasPlayedExplosionSound(false),
		_fireRateLevel(1),
		_fireSpreadLevel(1),
		_fireCountdown(sf::Time::Zero),
		_fireCommand(),
		_launchMissileCommand(),
		_dropPickupCommand(),
		_isMarkedForRemoval(false),
		_isLaunchingMissile(false),
		_missileAmmo(30),
		_spawnPickup(false)
	{
		_explosion.setFrameSize(sf::Vector2i(256,256));
		_explosion.setNumFrames(16);
		_explosion.setDuration(sf::seconds(1));
		centerOrigin(_explosion);
		centerOrigin(_sprite);//may not need this

		//set up commands
		_fireCommand.category = Category::Type::AirSceneLayer;
		_fireCommand.action = [this, &textures](SceneNode& node, sf::Time dt)
		{
			createBullets(node, textures);
		};

		//launch missile is an event
		_launchMissileCommand.category = Category::Type::AirSceneLayer;
		_launchMissileCommand.action = [this, &textures](SceneNode& node, sf::Time dt)
		{
			createProjectile(node, Projectile::Type::Missile, 0.f, 0.5f, textures);
		};

		_dropPickupCommand.category = Category::AirSceneLayer;
		_dropPickupCommand.action = [this, &textures](SceneNode& node, sf::Time dt)
		{
			createPickup(node, textures);
		};

		centerOrigin(_sprite);

		std::unique_ptr<TextNode> health(new TextNode(""));
		_healthDisplay = health.get();
		attachChild(std::move(health));

		std::unique_ptr<TextNode> missile(new TextNode(""));
		_missileDisplay = missile.get();
		attachChild(std::move(missile));

		updateTexts();
	}

	void GEX::Aircraft::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{
		if (isDestroyed() && _showExplosion)
		{
			target.draw(_explosion, states);
		}
		else {
			target.draw(_sprite, states);
		}
	}
	unsigned int Aircraft::getCategory() const
	{
		switch (_type)
		{
		case AircraftType::Eagle:
			return Category::Type::PlayerAircraft;
			break;
		case AircraftType::Raptor:
			return Category::Type::EnemyAircraft;
			break;
		case AircraftType::Avenger:
			return Category::EnemyAircraft;
			break;
		default:
			return Category::Type::None;
			break;
		}
	}
	void Aircraft::updateTexts()
	{
		_healthDisplay->setString(std::to_string(getHitPoints()) + " HP");
		_healthDisplay->setPosition(0.f, 50.f);
		_healthDisplay->setRotation(-getRotation());
		if (_type == AircraftType::Eagle)
		{
			_missileDisplay->setString("Missile: " + std::to_string(_missileAmmo));
			_missileDisplay->setPosition(0.f, 70.f);
		}
	}
	void Aircraft::fire()
	{
		if (TABLE.at(_type).fireInterval != sf::Time::Zero) {
			_isFiring = true;
		}
	}
	void Aircraft::launchMissile()
	{
		
			_isLaunchingMissile = true;
		
	}
	void Aircraft::playLocalSound(CommandQueue & commands, SoundEffectID effect)
	{
		Command playSoundCommand;
		playSoundCommand.category=Category::SoundEffect;
		playSoundCommand.action = derivedAction<SoundNode>(
			std::bind(&SoundNode::playSound, _1, effect, getWorldPosition()));

		commands.push(playSoundCommand);
	}
	bool Aircraft::isAllied() const
	{
		return _type== AircraftType::Eagle;//eagles are only allies
	}
	void Aircraft::increaceFireRate()
	{
		if (_fireRateLevel < 10)
		{
			_fireRateLevel++;
		}
	}
	void Aircraft::increaceFireSpread()
	{
		if (_fireSpreadLevel < 3)
		{
			_fireSpreadLevel++;
		}
	}
	void Aircraft::collectMissiles(unsigned int count)
	{
		_missileAmmo += count;
	}
	sf::FloatRect Aircraft::getBoundingBox() const
	{
		return getWorldTransform().transformRect(_sprite.getGlobalBounds());
	}
	bool Aircraft::isMarkedForRemoval() const
	{
		return (isDestroyed() && (_explosion.isFinished() || !_showExplosion));
	}
	void Aircraft::remove()
	{
		Entity::remove();
		_showExplosion = false;
	}
	void Aircraft::updateRollAnimation()
	{
		if (TABLE.at(_type).hasRollAnimation)
		{
			//default texture for no rolling
			sf::IntRect textureRect = TABLE.at(_type).textureRect;

			//rolling left move texture over one
			if (getVelocity().x < 0.f)
			{
				textureRect.left += textureRect.width;
			}

			//rolling right move texture over 2
			else if (getVelocity().x > 0.f)
			{
				textureRect.left += 2 * textureRect.width;
			}

			_sprite.setTextureRect(textureRect);
		}
	}
	void Aircraft::updateCurrent(sf::Time dt, CommandQueue& commands)
	{
		updateMovementPattern(dt);
		if (isDestroyed())
		{
			checkPickupDrop(commands);

			_explosion.update(dt);

			if (!_hasPlayedExplosionSound)
			{
				_hasPlayedExplosionSound = true;
				SoundEffectID effect = (randomInt(2) == 0 ? SoundEffectID::Explosion1 : SoundEffectID::Explosion2);
				playLocalSound(commands, effect);
			}
			

			return;
		}

		Entity::updateCurrent(dt,commands);
		updateTexts();
		updateRollAnimation();
		checkProjectileLaunch(dt, commands);
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
	void Aircraft::checkPickupDrop(CommandQueue & commands)
	{
		if (!isAllied() && randomInt(3) == 0 && !_spawnPickup)
		{
			commands.push(_dropPickupCommand);
		}
		_spawnPickup = true;
	}
	void Aircraft::createPickup(SceneNode & node, const TextureManager & textures) const
	{
		auto type = static_cast<Pickup::Type>(randomInt((static_cast<int>(Pickup::Type::Count))));

		std::unique_ptr<Pickup> pickup(new Pickup(type, textures));
		pickup->setPosition(getWorldPosition());
		//pickup->setVelocity(0.f, 1.f);
		pickup->setVelocity(0.f, 0.f);
		node.attachChild(std::move(pickup));
	}
	void Aircraft::createBullets(SceneNode & node, const TextureManager & textures)const
	{
		//if allied set to be ally otherwise set to be enemy bullet
		Projectile::Type type = isAllied() ? Projectile::Type::AlliedBullet : Projectile::Type::EnemyBullet;

		//change amount of bullets based on spread level
		switch (_fireSpreadLevel)
		{
		case 1:
			createProjectile(node, type, 0.0f, 0.5f, textures);//centered on x and .5f up the plane
			break;

		case 2:
			//two bullets
			createProjectile(node, type, -0.33f, 0.33f, textures);//bullets .33 to the left and right of plane
			createProjectile(node, type, 0.33f, 0.33f, textures);
			break;

		case 3:
			//three bullets
			createProjectile(node, type, -0.5f, 0.33f, textures);//bullet .5 to the left and right with a bullet in the center
			createProjectile(node, type, 0.0f, 0.5f, textures);
			createProjectile(node, type, 0.5f, 0.33f, textures);
			break;
		}
	}
	void Aircraft::createProjectile(SceneNode & node, Projectile::Type type, float xOffset, float yOffset,
		const TextureManager& textures)const
	{
		std::unique_ptr<Projectile> projectile(new Projectile(type, textures));

		sf::Vector2f offset(xOffset * _sprite.getGlobalBounds().width, yOffset * _sprite.getGlobalBounds().height);
		sf::Vector2f velocity(0, projectile->getMaxSpeed());

		//if allied it goes up, enemies go down 
		float sign = isAllied() ? -1.f : 1.f;

		//the offset is the distance from the firing plane
		projectile->setPosition(getWorldPosition() + offset * sign);
		projectile->setVelocity(velocity * sign);
		node.attachChild(std::move(projectile));

	}
	void Aircraft::checkProjectileLaunch(sf::Time dt, CommandQueue & commands)
	{
		//enemies that fire are always firing
		if (!isAllied())
		{
			fire();
		}

		//check if trying to fire and if allowed to fire with the interval
		if (_isFiring && _fireCountdown <= sf::Time::Zero)
		{
			commands.push(_fireCommand);
			playLocalSound(commands, isAllied() ? SoundEffectID::AlliedGunfire : SoundEffectID::EnemyGunfire);
			_isFiring = false;
			_fireCountdown += TABLE.at(_type).fireInterval / (_fireRateLevel + 1.f);
		}
		else if (_fireCountdown > sf::Time::Zero)//tried to fire but not allowed yet due to firing cooldown
		{
			_fireCountdown -= dt;
		}

		//missile firing
		if (_isLaunchingMissile)
		{
			//if you have missiles to launch then launch the missile and decreace the ammo amount
			if (_missileAmmo > 0)
			{
				commands.push(_launchMissileCommand);
				playLocalSound(commands, SoundEffectID::LaunchMissile);

				_isLaunchingMissile = false;
				_missileAmmo--;
			}
		}
	}
}