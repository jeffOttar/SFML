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
#include "MenuState.h"
#include "Utility.h"
#include "FontManager.h"

MenuState::MenuState(GEX::StateStack & stack, Context context) :
	State(stack, context),
	_options(),
	_optionsIndex(0)
{
	sf::Texture& texture = context.textures->get(GEX::TextureID::TitleScreen);

	_backgroundSprite.setTexture(texture);

	//set up the menu 
	//the play option settings
	sf::Text playOption;
	playOption.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
	playOption.setString("Play");
	centerOrigin(playOption);
	playOption.setPosition(context.window->getView().getSize() / 2.f);
	_options.push_back(playOption);//add the option to the list

	//exit option settings
	sf::Text exitOption;
	exitOption.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
	exitOption.setString("Exit");
	centerOrigin(exitOption);
	exitOption.setPosition(playOption.getPosition() + sf::Vector2f(0.f, 30.f));
	_options.push_back(exitOption);//add the option to the list

	//this updates the display to show current selection
	updateOptionText();
}

void MenuState::draw()
{
	sf::RenderWindow& window = *getContext().window;//get the window from the context

	window.setView(window.getDefaultView());
	window.draw(_backgroundSprite);

	//draw each of the text options
	for (const sf::Text& text : _options)
	{
		window.draw(text);
	}
}

bool MenuState::update(sf::Time dt)
{
	return true;
}

bool MenuState::handleEvent(const sf::Event & event)
{
	//if the event is not a keypress
	if (event.type != sf::Event::KeyPressed)
	{
		return false;
	}

	if (event.key.code == sf::Keyboard::Return)//if key press is return
	{
		if (_optionsIndex == Play)// if currently selected option is play
		{
			requestStackPop();
			requestStackPush(GEX::StateID::Game);
		}
		else if (_optionsIndex == Exit)
		{
			//remove the item from the stack so it is empty (pop will empty stack because the menu has nothing under it)
			requestStackPop();
		}
	}
	else if (event.key.code == sf::Keyboard::Up)//if key is up
	{
		//if they use the up key move the index in the correct direction for the selections
		if (_optionsIndex > 0)
		{
			_optionsIndex--;
		}
		else
		{
			_optionsIndex = _options.size() - 1;
		}

		updateOptionText();
	}
	else if (event.key.code == sf::Keyboard::Down)//if key is down
	{
		//if they use the up key move the index in the correct direction for the selections
		if (_optionsIndex < _options.size() - 1)// if it is less than the max size/ top of the vector
		{
			_optionsIndex++;
		}
		else//if it reaches the top then reset to the 0 indexed item
		{
			_optionsIndex = 0;
		}

		updateOptionText();
	}

	return true;
}

void MenuState::updateOptionText()
{
	if (_options.empty())
	{
		return;
	}

	for (sf::Text& text : _options)//set all text to white
	{
		text.setFillColor(sf::Color::White);
		text.setOutlineColor(sf::Color::Black);
	}
	
	//change the selected texts color
	_options[_optionsIndex].setFillColor(sf::Color::Magenta);
}
