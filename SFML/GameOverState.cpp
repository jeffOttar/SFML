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

#include "GameOverState.h"
#include "FontManager.h"
#include "Utility.h"
#include "GameState.h"


GameOverState::GameOverState(GEX::StateStack & stack, Context context) :
	State(stack, context),
	_gameOverText(),
	_elapsedTime(sf::Time::Zero)
{
	//set up gameover text
	sf::Font& font= GEX::FontManager::getInstance().get(GEX::FontID::Main);
	sf::Vector2f windowSize = context.window->getView().getSize();

	_gameOverText.setFont(font);

	if (context.player->getMissionStatus() == GEX::MissionStatus::MissionFailure)
	{
		_gameOverText.setString("Mission failed!");
	}
	else 
	{
		_gameOverText.setString("Mission successful!");
	}

	_gameOverText.setCharacterSize(70);
	centerOrigin(_gameOverText);	
	_gameOverText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);
}

void GameOverState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	//create background
	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 15));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(_gameOverText);
}

bool GameOverState::update(sf::Time dt)
{
	_elapsedTime += dt;
	if (_elapsedTime > sf::seconds(3))
	{
		requestStackClear();
		requestStackPush(GEX::StateID::Menu);
	}
	return false;
}

bool GameOverState::handleEvent(const sf::Event & event)
{
	return false;
}
