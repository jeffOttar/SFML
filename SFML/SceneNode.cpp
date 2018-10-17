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
#include "SceneNode.h"
#include <cassert>
#include "Command.h"
#include "CommandQueue.h"
#include <algorithm>

namespace GEX {

	SceneNode::SceneNode(Category::Type category) :
		_children(),
		_parent(nullptr),
		_category(category)
	{
	}


	void SceneNode::attachChild(Ptr child)
	{
		child->_parent = this;//this current thing is the childs parent
		_children.push_back(std::move(child));
	}

	SceneNode::Ptr SceneNode::detachChild(const SceneNode & ptr)//Ptr has to be scoped because the using statement is within the scenenode.h
	{
		auto found = std::find_if(_children.begin(), _children.end(), 
			[&ptr](Ptr& p) 
		{
			return &ptr == p.get();//compare the p with the reference of ptr
		}
		);
		assert(found != _children.end());

		Ptr result = std::move(*found);//found is an iterator so dereference it to get the value/object
		_children.erase(found);//erase based on iterator

		return result;
	}

	void SceneNode::update(sf::Time dt, CommandQueue& commands)
	{
		updateCurrent(dt,commands);
		updateChildren(dt,commands);
	}

	void SceneNode::onCommand(const Command & command, sf::Time dt)
	{
		//command current node if category matches
		if (command.category & getCategory())//uses bitwise AND
		{
			command.action(*this, dt);
		}

		//command children
		for (Ptr& child : _children) 
		{
			child->onCommand(command, dt);
		}
	}

	unsigned int SceneNode::getCategory() const
	{
		return _category;
	}

	sf::Vector2f SceneNode::getWorldPosition() const
	{
		return getWorldTransform() * sf::Vector2f();//applying the transformation to the identity vector gives the world position
	}

	sf::Transform SceneNode::getWorldTransform() const
	{
		sf::Transform transform = sf::Transform::Identity;//this is a constant part of sf transform definition(it is a transform that doesnt change anything) it is a starting point
		//it allows us to multiply in additional transformations

		for (const SceneNode* node = this; node != nullptr; node = node->_parent)//start pointing at self and go until at end of nodes
		{
			transform = node->getTransform() * transform;
		}

		return transform;
	}

	void SceneNode::updateCurrent(sf::Time dt, CommandQueue& commands)
	{
		//default it does nothing but it is also inherited
	}

	void SceneNode::updateChildren(sf::Time dt, CommandQueue& commands)
	{
		for (Ptr& child : _children)//this isnt const like draw as with draw you dont change the state but with update you do
		{
			child->update(dt,commands);
		}
	}

	void SceneNode::draw(sf::RenderTarget & target, sf::RenderStates states) const
	{
		states.transform *= getTransform();//getTransform gets all the transforms done to the node---they are multiplied so that they are added into the states
		//above adds your transforms to the states

		drawCurrent(target, states);
		drawChildren(target, states);
	}

	void SceneNode::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{
		//to be overwritten by objects that take scene node
	}

	void SceneNode::drawChildren(sf::RenderTarget & target, sf::RenderStates states) const
	{
		for (const Ptr& child : _children)
		{
			child->draw(target, states);
		}
	}

}
