/**
* @file
* @author Jeff Ottar-
* @version 1.0
*
*
* @section DESCRIPTION
* 
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

#include <SFML/System/Time.hpp>
#include <functional>//for std::function 
#include <cassert>

namespace GEX {

	//forward declaration
	class SceneNode;

	struct Command
	{
	public:
		Command();
		
		//command object(structure made of action and target)
		//(action is a functor, target is the node on the scenegraph it is)

		std::function <void(SceneNode&, sf::Time) >		action;//action is a member that is void and takes a scenenode and time
		unsigned int									category;//category is used as bitwise flags to indicate categories so that you can target a single category or multiple categories at once

	};
	template <typename GameObject, typename Function>
	std::function<void(SceneNode&, sf::Time)>			derivedAction(Function fn)//this is a function called derivedaction --> it takes a function and returns a function with the correct signature to be an action so that you can use it for any object on the scenenode
	{
		return [=]//scenenode and time captured by value
		(SceneNode& node, sf::Time dt) 
		{
			assert(dynamic_cast<GameObject*>(&node) != nullptr);//says you are sure that you can dynamicly cast this (assert is used for debugging)

			fn(static_cast<GameObject&>(node), dt);//this part will change the object to the needed thing
		};
	}
}

