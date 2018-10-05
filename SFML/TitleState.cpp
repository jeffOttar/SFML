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
#include "TitleState.h"
#include "Utility.h"
#include "FontManager.h"

TitleState::TitleState(GEX::StateStack & stack, Context context) :
	State(stack, context),
	_text(),
	_showText(true),
	_textEffectTime(sf::Time::Zero)
{
	_backgroundSprite.setTexture(context.textures->get(GEX::TextureID::TitleScreen));

	_text.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
	_text.setString("Press any key to start");

	centerOrigin(_text);
	_text.setPosition(context.window->getView().getSize() / 2.f);
}

void TitleState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.draw(_backgroundSprite);

	if (_showText)//this is for drawing the text when it should be drawn
	{
		window.draw(_text);
	}
}

bool TitleState::update(sf::Time dt)
{
	_textEffectTime += dt;

	//this is to make the text blink it will flash every half second
	if (_textEffectTime >= sf::seconds(0.5f))
	{
		_showText = !_showText;//this switches the value of show text between true and false
		_textEffectTime = sf::Time::Zero;
	}

	return true;
}

bool TitleState::handleEvent(const sf::Event & event)
{
	//if the user presses a key then progress on to the menu
	if (event.type == sf::Event::KeyPressed)
	{
		requestStackPop();
		requestStackPush(GEX::StateID::Menu);
	}

	return true;
}
