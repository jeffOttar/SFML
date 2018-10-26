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
#include <SFML/Graphics/Sprite.hpp>

#include "TextureManager.h"
#include "Entity.h"
#include "Command.h"
#include "Projectile.h"
#include "Animation.h"

namespace GEX {
	class TextNode;
	class CommandQueue;
	enum class AircraftType
	{
		Eagle,
		Raptor,
		Avenger
	};

	class Aircraft : public Entity
	{
	public:
		Aircraft(AircraftType type, const TextureManager& textures);

		virtual void		drawCurrent(sf::RenderTarget& target, sf::RenderStates states)const override;

		unsigned int		getCategory() const override;

		void				updateTexts();//update the mini health and Missile display

		void				fire();
		void				launchMissile();

		bool				isAllied() const;

		void				increaceFireRate();
		void				increaceFireSpread();
		void				collectMissiles(unsigned int count);
		sf::FloatRect		getBoundingBox() const override;
		bool 				isMarkedForRemoval() const override;

		void				remove() override;


	protected:
		void				updateCurrent(sf::Time dt, CommandQueue& commands) override;

	private:
		void				updateMovementPattern(sf::Time dt);
		float				getMaxSpeed() const;


		void				checkPickupDrop(CommandQueue& commands);
		void				createPickup(SceneNode& node, const TextureManager& textures) const;

		void				createBullets(SceneNode& node, const TextureManager& textures)const;
		void				createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, 
												const TextureManager& textures)const;

		void				checkProjectileLaunch(sf::Time dt, CommandQueue& commands);

	private:
		AircraftType		_type;
		sf::Sprite			_sprite;
		Animation			_explosion;
		bool				_showExplosion;

		TextNode*			_healthDisplay;
		TextNode*			_missileDisplay;

		float				_travelDistance;
		std::size_t			_directionIndex;

		bool				_isFiring;

		int					_fireRateLevel;//fire 1 2 or 3 
		int					_fireSpreadLevel;

		sf::Time			_fireCountdown;

		Command				_fireCommand;
		Command				_launchMissileCommand;
		Command				_dropPickupCommand;

		bool				_isMarkedForRemoval;

		bool				_isLaunchingMissile;
		int					_missileAmmo;

		bool				_spawnPickup;
	};
}
