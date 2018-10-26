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

#include "EmitterNode.h"
#include "CommandQueue.h"
#include "Command.h"

namespace GEX {
	EmitterNode::EmitterNode(Particle::Type type) :
		SceneNode(),
		_accumulatedTime(sf::Time::Zero),
		_type(type),
		_particleSystem(nullptr)
	{}
	void EmitterNode::updateCurrent(sf::Time dt, CommandQueue & commands)
	{
		//if you have a particle system you can emit the particles in it
		if (_particleSystem)
		{
			emitParticles(dt);
		}
		else
		{
			//find the particle node with the same type as the emitter node
			auto finder = [this](ParticleNode& container, sf::Time)
			{
				if (container.getParticleType() == _type)
					_particleSystem = &container;
			};

			//make the command to find the particle system
			Command command;
			command.category = Category::ParticleSystem;
			command.action = derivedAction<ParticleNode>(finder);

			commands.push(command);
		}
	}
	void EmitterNode::emitParticles(sf::Time dt)
	{
		const float EMISSION_RATE = 30.f;
		const sf::Time INTERVAL = sf::seconds(1.f) / EMISSION_RATE;

		_accumulatedTime += dt;

		while (_accumulatedTime > INTERVAL)
		{
			_accumulatedTime -= INTERVAL;
			//add the particles to the system relative to where the object is in the system
			//so the particles will be located by the missile or other object it comes from
			_particleSystem->addParticle(getWorldPosition());}
	}
}