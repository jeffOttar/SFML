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
#pragma once

#include "StateIdentifiers.h"
#include "State.h"
#include <functional>

//forward declaration for sf objects (need to be in namespace as they are in the sf namespace)
namespace sf{
	class Event;
	class RenderWindow;
}

namespace GEX {

	class StateStack
	{
	public:

		enum class Action 
		{
			Push, 
			Pop,
			Clear
		};

							//context is passed around by value
		explicit			StateStack(State::Context context);
		~StateStack();

		template <typename T>
		void				registerState(GEX::StateID stateID);

		void				draw();
		void				update(sf::Time dt);
		void				handleEvent(const sf::Event& event);

		void				pushState(GEX::StateID stateID);
		void				popState();
		void				clearStates();

		bool				isEmpty() const;

	private:
		State::Ptr			createState(GEX::StateID stateID);
		void				applyPendingChanges();

	private:
		struct PendingChange
		{
			explicit	PendingChange(Action action, StateID stateID=GEX::StateID::None);

			Action													action;
			GEX::StateID											stateID;
		};

	private:
		std::vector<State::Ptr>										_stack;
		std::vector<PendingChange>									_pendingList;
		State::Context												_context;
		std::map< GEX::StateID, std::function<State::Ptr()> >		_factories;
	};

	template<typename T>
	void StateStack::registerState(GEX::StateID stateID)
	{
		_factories[stateID] = [this]() 
		{
			//you are making a new blank state represented by the template t where t is the desired state
			return State::Ptr(new T(*this,_context));
			//pass in the stateStack/this and the context
		};
	}
}