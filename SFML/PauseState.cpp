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
#include "PauseState.h"
#include "Utility.h"
#include "FontManager.h"

PauseState::PauseState(GEX::StateStack & stack, Context context) :
	State(stack, context),
	_backgroundSprite(),
	_pausedText(),
	_instructionText()
{

	//set up the paused text
	_pausedText.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
	_pausedText.setString("Game Paused");
	_pausedText.setCharacterSize(80);
	centerOrigin(_pausedText);

	//set up the instruction text
	_instructionText.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
	_instructionText.setString("(Press Backspace to return to the main menu)");
	centerOrigin(_instructionText);

	//set position of the text
	sf::Vector2f viewSize = context.window->getView().getSize();
	_pausedText.setPosition(0.5f * viewSize.x, 0.4f * viewSize.y);
	_instructionText.setPosition(0.5f * viewSize.x, 0.6f * viewSize.y);
}

void PauseState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(_pausedText);
	window.draw(_instructionText);
}

bool PauseState::update(sf::Time dt)
{
	return false;
}

bool PauseState::handleEvent(const sf::Event & event)
{
	if (event.type != sf::Event::KeyPressed)
	{
		return false;
	}

	if (event.key.code == sf::Keyboard::Escape)
	{
		//if you press escape return to the game by popping the stack with the pausestate on it
		requestStackPop();
	}

	if (event.key.code == sf::Keyboard::BackSpace)
	{
		//clear the stack and go to the menu if backspace pressed
		requestStackClear();
		requestStackPush(GEX::StateID::Menu);
	}

	return false;
}
