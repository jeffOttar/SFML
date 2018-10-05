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
#include "GexState.h"
#include "Utility.h"
#include "FontManager.h"

GexState::GexState(GEX::StateStack & stack, Context context):
State(stack,context),
_backgroundSprite(),
_pausedText(),
_instructionGText(),
_exitInstructionText(),
_stateText()
{
	//set the background face texture
	_backgroundSprite.setTexture(context.textures->get(GEX::TextureID::Face));
	centerOrigin(_backgroundSprite);
	_backgroundSprite.setColor(sf::Color(255, 255, 255, 100));//make image transparent

	//set up the paused text
	_pausedText.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
	_pausedText.setString("Game Paused");
	_pausedText.setCharacterSize(60);
	centerOrigin(_pausedText);

	//set up the state text title
	_stateText.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
	_stateText.setString("GEX State");
	_stateText.setCharacterSize(40);
	centerOrigin(_stateText);


	//set up the G key instruction text
	_instructionGText.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
	_instructionGText.setString("(Press G to return to Game)");
	centerOrigin(_instructionGText);

	//set up the quit instruction text
	_exitInstructionText.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
	_exitInstructionText.setString("(Press Backspace to return to the main menu)");
	centerOrigin(_exitInstructionText);

	//set position of the objects
	sf::Vector2f viewSize = context.window->getView().getSize();
	_pausedText.setPosition(0.5f * viewSize.x, 0.2f * viewSize.y);
	_stateText.setPosition(0.5f * viewSize.x, 0.4f * viewSize.y);
	_instructionGText.setPosition(0.5f * viewSize.x, 0.6f * viewSize.y);
	_exitInstructionText.setPosition(0.5f * viewSize.x, 0.8f * viewSize.y);
	_backgroundSprite.setPosition(viewSize / 2.f);//center position

}

//draws objects
void GexState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;//the translucent background
	backgroundShape.setFillColor(sf::Color(255, 0, 0, 100));
	backgroundShape.setSize(window.getView().getSize());

	//draw objects
	window.draw(backgroundShape);
	window.draw(_backgroundSprite);
	window.draw(_pausedText);
	window.draw(_exitInstructionText);
	window.draw(_instructionGText);
	window.draw(_stateText);
}

//dont update as it is like the pause state and doesnt need to be updated
bool GexState::update(sf::Time dt)
{
	return false;
}

//handle the key events
bool GexState::handleEvent(const sf::Event & event)
{
	if (event.type != sf::Event::KeyPressed)
	{
		return false;
	}

	if (event.key.code == sf::Keyboard::G)
	{
		//return to the game
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
