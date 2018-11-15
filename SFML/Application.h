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
* Copyright 2018
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

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

#include "PlayerControl.h"
#include "TextureManager.h"
#include "StateStack.h"
#include "MusicPlayer.h"
#include "SoundPlayer.h"

class Application
{
public:
							Application();

	void					run();

private:
	void					processInput();
	void					update(sf::Time timeElapsed);
	void					render();

	void					updateStats(sf::Time deltaTime);
	void					registerStates();


private:
	//these are created in the order they are declared here
	static const sf::Time	TimePerFrame;

	sf::RenderWindow		_window;
	GEX::PlayerControl		_player;
	sf::Font				_font;
	GEX::TextureManager		_textures;
	GEX::StateStack			_stateStack;//state stack needs the above objects so it must be declared afterwards
	GEX::MusicPlayer		_music;
	GEX::SoundPlayer		_sound;

	sf::Text				_statisticsText;
	sf::Time				_statisticsUpdateTime;
	unsigned int			_statisticsNumFrames;
};

