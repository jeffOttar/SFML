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

#include "Projectile.h"
#include "DataTables.h"
#include "Utility.h"
#include "Category.h"


namespace GEX {
	namespace
	{
		const std::map<Projectile::Type, ProjectileData>TABLE = initializeProjectileData();
	}

	Projectile::Projectile(Type type, const TextureManager & textures) :
		Entity(1),
		_type(type),
		_sprite(textures.get(TABLE.at(type).texture)),
		_targetDirection()
	{
		centerOrigin(_sprite);
	}
	unsigned int Projectile::getCategory() const
	{
		if (_type == Type::EnemyBullet) {
			return Category::Type::EnemyProjectile;
		}
		else {
			return Category::Type::AlliedProjectile;
		}
	}
	float Projectile::getMaxSpeed() const
	{
		return TABLE.at(_type).speed;
	}
	int Projectile::getDamage() const
	{
		return TABLE.at(_type).damage;
	}
	bool Projectile::isGuided() const
	{
		return (_type == Type::Missile);
	}
	void Projectile::guidedTowards(sf::Vector2f position)
	{
		assert(isGuided());//assert it is a missile
		//make a unit vector out of the position of target - your position
		_targetDirection = unitVector(position - getWorldPosition());
	}
	sf::FloatRect Projectile::getBoundingBox() const
	{
		return getWorldTransform().transformRect(_sprite.getGlobalBounds());
	}
	void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands)
	{
		if (isGuided())
		{
			//const float APPROACH_RATE = 40.f;//missile approach rate//to slow
			//const float APPROACH_RATE = 400.f;//missile approach rate//to fast
			const float APPROACH_RATE = 100.f;//missile approach rate

			//unit vector is direction
			auto newVelocity = unitVector(APPROACH_RATE * dt.asSeconds() *
				_targetDirection + getVelocity());

			newVelocity *= getMaxSpeed();
			setVelocity(newVelocity);

			auto angle = std::atan2(newVelocity.y, newVelocity.x);
			setRotation(toDegree(angle) + 90.f);
		}

		Entity::updateCurrent(dt, commands);
	}
	void Projectile::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{
		target.draw(_sprite, states);
	}
}