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

#include "SoundPlayer.h"

#include <SFML/Audio/Listener.hpp>

#include <cassert>

namespace {
	// Sound coordinate system, point of view of a player in front of the screen:
	// X = left; Y = up; Z = back (out of the screen)
	const float ListenerZ = 300.f;
	const float Attenuation = 8.f;
	const float MinDistance2D = 200.f;
	const float MinDistance3D = std::sqrt(MinDistance2D*MinDistance2D + ListenerZ * ListenerZ);
}
namespace GEX {
	SoundPlayer::SoundPlayer():
		_soundBuffers(),
		_sounds()
	{
		loadBuffer(SoundEffectID::AlliedGunfire, "Media/Sound/AlliedGunfire.wav");
		loadBuffer(SoundEffectID::EnemyGunfire, "Media/Sound/EnemyGunfire.wav");
		loadBuffer(SoundEffectID::Explosion1, "Media/Sound/Explosion1.wav");
		loadBuffer(SoundEffectID::Explosion2, "Media/Sound/Explosion2.wav");
		loadBuffer(SoundEffectID::LaunchMissile, "Media/Sound/LaunchMissile.wav");
		loadBuffer(SoundEffectID::CollectPickup, "Media/Sound/CollectPickup.wav");
		loadBuffer(SoundEffectID::Button, "Media/Sound/Button.wav");

		// Listener points towards the screen (default in SFML)
		sf::Listener::setDirection(0.f, 0.f, -1.f);
	}

	void SoundPlayer::play(SoundEffectID effect)
	{
		play(effect, getListenerPosition());
	}

	void SoundPlayer::play(SoundEffectID effect, sf::Vector2f position)
	{
		_sounds.push_back(sf::Sound());//add a sound to the list
		sf::Sound& sound = _sounds.back();//get the sound to modify newly added sound
		sound.setBuffer(*_soundBuffers[effect]);
		sound.setPosition(position.x,-position.y,0.f);
		sound.setAttenuation(Attenuation);
		sound.setMinDistance(MinDistance3D);

		sound.play();
	}

	void SoundPlayer::removeStoppedSounds()
	{
		//remove a sf sound if it has stopped
		_sounds.remove_if([](const sf::Sound& s)
		{
			return s.getStatus() == sf::Sound::Stopped;
		});
	}

	void SoundPlayer::setListenerPosition(sf::Vector2f position)
	{
		sf::Listener::setPosition(position.x, -position.y, ListenerZ);//flip the y
	}

	sf::Vector2f SoundPlayer::getListenerPosition() const
	{
		sf::Vector3f pos = sf::Listener::getPosition();
		return sf::Vector2f(pos.x, -pos.y);//flip y back into correct way
	}

	void SoundPlayer::loadBuffer(SoundEffectID id, const std::string path)
	{
		std::unique_ptr<sf::SoundBuffer> buffer(new sf::SoundBuffer);
		if (!buffer->loadFromFile(path))
		{
			throw std::runtime_error("Sound Effect Load Failed");
		}

		auto inserted = _soundBuffers.insert(std::make_pair(id, std::move(buffer)));
		assert(inserted.second);//assert second is true --> insert first member points to pair, second points to whether it was successful
	}
}