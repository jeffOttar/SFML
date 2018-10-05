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

#include "StateStack.h"
#include <cassert>


GEX::StateStack::StateStack(State::Context context) :
	_stack(),
	_pendingList(),
	_context(context),
	_factories()
{}

GEX::StateStack::~StateStack()
{}

void GEX::StateStack::draw()
{
	//the stack has a bunch of iterators to state
	for (State::Ptr& state : _stack)
	{
		state->draw();//draw from the bottom up
	}
}

void GEX::StateStack::update(sf::Time dt)
{
		//iterate through the reverse order for each itr updating each that returns true
		for (auto itr = _stack.rbegin(); itr != _stack.rend(); itr++)
		{
			if (!(*itr)->update(dt))//the -> is stickier than the asterisk so surround *itr in brackets
			{
				break;
			}
		}
		applyPendingChanges();
}

void GEX::StateStack::handleEvent(const sf::Event & event)
{
	for (auto itr = _stack.rbegin(); itr != _stack.rend(); itr++)
	{
		//if it == false then break
		if (!(*itr)->handleEvent(event))//the -> is stickier than the asterisk so surround *itr in brackets
		{
			break;
		}
	}

	applyPendingChanges();
}

void GEX::StateStack::pushState(GEX::StateID stateID)
{
	_pendingList.push_back(PendingChange(Action::Push, stateID));
}

void GEX::StateStack::popState()
{
	_pendingList.push_back(PendingChange(Action::Pop));
}

void GEX::StateStack::clearStates()
{
	_pendingList.push_back(PendingChange(Action::Clear));
}

bool GEX::StateStack::isEmpty() const
{
	return _stack.empty();
}

GEX::State::Ptr GEX::StateStack::createState(GEX::StateID stateID)
{
	auto found = _factories.find(stateID);
	assert(found != _factories.end());

	//return found second value which is a function that will execute and give the instance that you desire
	return found->second();
}

void GEX::StateStack::applyPendingChanges()
{
	//go through the pending changes and do each action
	for (auto change : _pendingList)
	{
		switch (change.action)
		{
		case Action::Push:
			_stack.push_back(createState(change.stateID));
			break;
		case Action::Pop: 
			_stack.pop_back();
			break;
		case Action::Clear: 
			_stack.clear();
			break;
		}
	}

	//clear the pending list after all pending actions have been completed
	_pendingList.clear();
}

GEX::StateStack::PendingChange::PendingChange(Action action, StateID stateID):
	action(action),
	stateID(stateID)
	
{
}
