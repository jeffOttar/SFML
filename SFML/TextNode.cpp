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
#include "TextNode.h"
#include "Utility.h"
#include "FontManager.h"

#include <SFML/Graphics/RenderTarget.hpp>


namespace GEX
{
	TextNode::TextNode(const std::string & text)
	{
		_text.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
		_text.setCharacterSize(20);
		setString(text);
	}
	void TextNode::setString(const std::string & text)
	{
		_text.setString(text);
		centerOrigin(_text);
	}
	void TextNode::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{
		target.draw(_text, states);
	}
}


