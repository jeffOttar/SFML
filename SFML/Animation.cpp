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

#include "Animation.h"
#include <SFML/Graphics/RenderTarget.hpp>

namespace GEX {

	Animation::Animation():
		_sprite(),
		_frameSize(),
		_numberOfFrames(0),
		_currentFrame(0),
		_duration(sf::Time::Zero),
		_elapsedTime(sf::Time::Zero),
		_repeat(false)
	{
	}

	Animation::Animation(const sf::Texture & texture) :
		_sprite(texture),
		_frameSize(),
		_numberOfFrames(0),
		_currentFrame(0),
		_duration(sf::Time::Zero),
		_elapsedTime(sf::Time::Zero),
		_repeat(false)
	{
	}

	void Animation::setTexture(const sf::Texture & texture)
	{
		_sprite.setTexture(texture);
	}

	const sf::Texture * Animation::getTexture() const
	{
		return _sprite.getTexture();
	}

	void Animation::setFrameSize(sf::Vector2i frameSize)
	{
		_frameSize = frameSize;
	}

	sf::Vector2i Animation::getFrameSize() const
	{
		return _frameSize;
	}

	void Animation::setNumFrames(std::size_t numFrames)
	{
		_numberOfFrames = numFrames;
	}

	std::size_t Animation::getNumFrames() const
	{
		return _numberOfFrames;
	}

	void Animation::setDuration(sf::Time duration)
	{
		_duration = duration;
	}

	sf::Time Animation::getDuration() const
	{
		return _duration;
	}

	void Animation::setRepeating(bool flag)
	{
		_repeat = flag;
	}

	bool Animation::isRepeating() const
	{
		return _repeat;
	}

	void Animation::restart()
	{
		_currentFrame = 0;
	}

	bool Animation::isFinished() const
	{
		return _currentFrame >= _numberOfFrames;//if current frame is equal to or past last frame
	}

	sf::FloatRect Animation::getLocalBounds() const
	{
		return sf::FloatRect(getOrigin(), static_cast<sf::Vector2f>(getFrameSize()));
	}

	sf::FloatRect Animation::getGlobalBounds() const
	{
		return getTransform().transformRect(getLocalBounds());
	}

	void Animation::update(sf::Time dt)
	{
		/*
		* If we didnt have equally ditributed evenly packed textures we could use a vector of texture rects and itterate through it
		*/
		//cast the number of frames as a float to do float division (not integer division)
		sf::Time timePerFrame = _duration / static_cast<float>(_numberOfFrames);
		_elapsedTime += dt;

		sf::Vector2i textureBounds(_sprite.getTexture()->getSize());//get the size of the texture for the bounds
		sf::IntRect textureRect = _sprite.getTextureRect();

		//if current frame is 0 set the texture rect 0,0 starts at top left corner of the sprite sheet
		//the other 2 values are the x and y for size
		if (_currentFrame == 0) 
		{
			textureRect = sf::IntRect(0, 0, _frameSize.x, _frameSize.y);
		}

		//while you used up the time for the frame and if there are still frames to show
		while (_elapsedTime >= timePerFrame && (_currentFrame <= _numberOfFrames || _repeat)) 
		{
			//move on to next frame
			textureRect.left += textureRect.width;
			//if the texture goes off sprite sheet then you need to go to the next row
			//if at end of y then it either ends or repeats
			if (textureRect.left + textureRect.width > textureBounds.x) 
			{ 
				textureRect.left = 0;//reset to the left
				textureRect.top += textureRect.height; //move onto the next row
			}

			_elapsedTime -= timePerFrame;    

			if (_repeat)
			{ 
				_currentFrame = (_currentFrame + 1) % _numberOfFrames;
				//if repeating and at the end of sprite sheet reset to beginning of sprite sheet
				if (_currentFrame == 0) 
				{
					textureRect = sf::IntRect(0, 0, _frameSize.x, _frameSize.y);
				}
			}
			else
			{
				_currentFrame++;
			}
		}
		//set the texture rect that will display the desired frame
		_sprite.setTextureRect(textureRect);
		

	}

	void Animation::draw(sf::RenderTarget & target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(_sprite, states);
	}
}