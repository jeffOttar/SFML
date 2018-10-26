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
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>


namespace GEX {
	class Animation :
		public sf::Drawable, public sf::Transformable
	{
	public:
									Animation();
									Animation(const sf::Texture& texture);

		void						setTexture(const sf::Texture& texture);
		const sf::Texture*			getTexture() const;

		void						setFrameSize(sf::Vector2i frameSize);
		sf::Vector2i				getFrameSize() const;

		void						setNumFrames(std::size_t numFrames);
		std::size_t					getNumFrames() const;

		void						setDuration(sf::Time duration);
		sf::Time					getDuration() const;

		void						setRepeating(bool flag);
		bool						isRepeating() const;

		void						restart();
		bool						isFinished() const;

		sf::FloatRect				getLocalBounds() const;
		sf::FloatRect				getGlobalBounds() const;

		void						update(sf::Time dt);

	private:
		void						draw(sf::RenderTarget& target, sf::RenderStates states)const override;//overide draw in the sf::drawable

	private:
		sf::Sprite		_sprite;
		sf::Vector2i	_frameSize;
		std::size_t		_numberOfFrames;
		std::size_t		_currentFrame;
		sf::Time		_duration;//duration of entire animation
		sf::Time		_elapsedTime;//time gone by since last frame
		bool			_repeat;//if animation repeats
	};

}