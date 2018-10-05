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
#include "PlayerControl.h"
#include "Aircraft.h"
#include "Command.h"
#include "CommandQueue.h"

namespace GEX {
	struct AircraftMover
	{
	public:
		AircraftMover(float vx, float vy) : //the constructor for the struct
			velocity(vx, vy) 
		{
		}
		void operator() (Aircraft& aircraft, sf::Time dt) const
		{
			aircraft.accelerate(velocity);
		}

		sf::Vector2f velocity;//this is accessible
	};

	PlayerControl::PlayerControl()
	{
		//set up key bindings
		_keyBindings[sf::Keyboard::Left] = Action::MoveLeft;
		_keyBindings[sf::Keyboard::Right] = Action::MoveRight;
		_keyBindings[sf::Keyboard::Up] = Action::MoveUp;
		_keyBindings[sf::Keyboard::Down] = Action::MoveDown;

		//alternate key bindings
		//_keyBindings[sf::Keyboard::A] = Action::MoveLeft;
		//_keyBindings[sf::Keyboard::D] = Action::MoveRight;
		//_keyBindings[sf::Keyboard::W] = Action::MoveUp;
		//_keyBindings[sf::Keyboard::S] = Action::MoveDown;

		_keyBindings[sf::Keyboard::R] = Action::RR;
		_keyBindings[sf::Keyboard::L] = Action::RL;
		_actionBindings[Action::RL].action = derivedAction<Aircraft>([](Aircraft& node, sf::Time dt) {node.rotate(10.f); });
		_actionBindings[Action::RR].action = derivedAction<Aircraft>([](Aircraft& node, sf::Time dt) {node.rotate(-10.f); });
		//set up action bindings
		initializeActions();

		for (auto& pair : _actionBindings)//sets the action bindings to the player aircraft
		{
			pair.second.category = Category::PlayerAircraft;
		}
	}

	///events are the one time events like firing a missle
	void PlayerControl::handleEvent(const sf::Event & event, CommandQueue & commands)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			auto found = _keyBindings.find(event.key.code);
			if (found != _keyBindings.end())
			{
				commands.push(_actionBindings[found->second]);
			}
		}
	}
	void PlayerControl::handleRealTimeInput(CommandQueue & commands)
	{
		//traverse all the assigned keys, look up the action, generate the command
		for (auto pair : _keyBindings)
		{
			if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
			{
				commands.push(_actionBindings[pair.second]);
			}
		}
	}
	void PlayerControl::initializeActions()
	{
		const float playerSpeed = 200.f;

		_actionBindings[Action::MoveLeft].action = derivedAction<Aircraft>(AircraftMover(-playerSpeed,0.f));
		_actionBindings[Action::MoveRight].action = derivedAction<Aircraft>(AircraftMover(playerSpeed, 0.f));
		_actionBindings[Action::MoveUp].action = derivedAction<Aircraft>(AircraftMover(0.f,-playerSpeed));
		_actionBindings[Action::MoveDown].action = derivedAction<Aircraft>(AircraftMover(0.f,playerSpeed));
	
	}
	bool PlayerControl::isRealtimeAction(Action action)
	{
		switch (action)
		{
		case Action::MoveLeft: 
		case Action::MoveRight: 
		case Action::MoveUp: 
		case Action::MoveDown: 
		case Action::RL:
		case Action::RR:
			return true;//if the action is any of the realtime actions return true
			break;
		default:
			return false;
		}
	}

}						  