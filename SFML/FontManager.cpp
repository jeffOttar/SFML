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

#include "FontManager.h"
#include <cassert>


namespace GEX {
	//this is how you initialize the class variables
	FontManager* FontManager::_instance = nullptr;

	FontManager & FontManager::getInstance()
	{
		//this is lazy instantiation where it is only made when asked for
		if (!_instance)
		{
			FontManager::_instance = new FontManager();
		}
		
		return *FontManager::_instance;//dereference it because it is a pointer and scope it because it is static
		
	}
	void FontManager::load(FontID id, const std::string & path)
	{
		std::unique_ptr<sf::Font> font(new sf::Font);//create new local font object in a ptr

		
		if (!font->loadFromFile(path))
		{
			throw std::runtime_error("Font Load Failed:" + path);
		}
		
		auto rc = _fonts.insert(std::make_pair(id, std::move(font)));

		if (!rc.second)//second is the value of true or false for whether it failed
		{
			assert(0);//assert(0) forces the program to crash
		}
	}
	sf::Font & FontManager::get(FontID id) const
	{
		auto found = _fonts.find(id);

		assert(found != _fonts.end());

		return *found->second;//dereference to get the font object
	}
}

