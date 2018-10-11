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
			return Category::EnemyProjectile;
		}
		else {
			return Category::AlliedProjectile;
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
	void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands)
	{
		Entity::updateCurrent(dt, commands);
	}
	void Projectile::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{
		target.draw(_sprite, states);
	}
}