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
#include "Utility.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

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
		if (found != _children.end()) {
			Ptr result = std::move(*found);//found is an iterator so dereference it to get the value/object
			_children.erase(found);//erase based on iterator

			return result;
		}
		return nullptr;
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

	sf::FloatRect SceneNode::getBoundingBox() const
	{
		return sf::FloatRect();//it is overriden for when the object has bounding box
	}

	void SceneNode::drawBoundingBox(sf::RenderTarget & target, sf::RenderStates states) const
	{
		sf::FloatRect rect = getBoundingBox();

		sf::RectangleShape box;
		box.setPosition(sf::Vector2f(rect.left, rect.top));
		box.setSize(sf::Vector2f(rect.width, rect.height));
		box.setFillColor(sf::Color::Transparent);
		box.setOutlineColor(sf::Color::Cyan);
		box.setOutlineThickness(1.f);

		target.draw(box);
	}

	void SceneNode::checkSceneCollision(SceneNode & rootNode, std::set<Pair>& collisionPair)
	{
		checkNodeCollision(rootNode, collisionPair);//check for node collision on the item

		//for every child in the root node children check for collision
		for (Ptr& c : rootNode._children) {
			checkSceneCollision(*c, collisionPair);
		}
	}

	void SceneNode::checkNodeCollision(SceneNode & node, std::set<Pair>& collisionPair)
	{
		//check to make sure you are not comparing yourself to yourself and then check if collision between yourself and new node
		if (this != &node && collision(*this, node) && !isDestroyed() && !node.isDestroyed()) {
			collisionPair.insert(std::minmax(this, &node));//insert a pair of pointers into the collisionPair if there is one
		}

		//check for collisions for each child
		for (Ptr& c : _children) {
			c->checkNodeCollision(node, collisionPair);
		}
	}

	bool SceneNode::isDestroyed() const
	{
		//by default you do not need to destroy the object
		return false;
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

		drawBoundingBox(target, states);
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

	float distance(const SceneNode & lhs, const SceneNode & rhs)
	{
		return length(lhs.getWorldPosition() - rhs.getWorldPosition());
	}

	bool collision(const SceneNode & lhs, const SceneNode & rhs)
	{
		//if one bounding box intersects another
		return lhs.getBoundingBox().intersects(rhs.getBoundingBox());
	}

}
