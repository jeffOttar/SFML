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

#include "ParticleNode.h"


namespace GEX {
	namespace
	{
		const std::map<Particle::Type, ParticleData>TABLE = initializeParticleData();
	}

	ParticleNode::ParticleNode(Particle::Type type, const TextureManager& textures) :
		SceneNode(),
		_particles(),
		_texture(textures.get(TextureID::Particle)),
		_type(type),
		_vertexArray(sf::Quads),
		_needsVertexUpdate(true)
	{
	}
	void ParticleNode::addParticle(sf::Vector2f position)
	{
		//everytime you add a particle get its position color and lifetime then add it to the particles deque
		Particle particle;
		particle.position = position;
		particle.color = TABLE.at(_type).color;
		particle.lifetime = TABLE.at(_type).lifetime;

		_particles.push_back(particle);
	}
	Particle::Type ParticleNode::getParticleType() const
	{
		return _type;
	}
	unsigned int ParticleNode::getCategory() const
	{
		return Category::ParticleSystem;
	}
	void ParticleNode::updateCurrent(sf::Time dt, CommandQueue & commands)
	{
		//while there are particles with no lifetime at the front remove them
		while (!_particles.empty() && _particles.front().lifetime <= sf::Time::Zero)
		{
			_particles.pop_front();
		}

		//decreace lifetime of still existing particles
		for (auto& p : _particles)
		{
			p.lifetime -= dt;
		}

		//mark for update
		_needsVertexUpdate = true;
	}
	void ParticleNode::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{
		if (_needsVertexUpdate)
		{
			computeVertices();
			_needsVertexUpdate = false;
		}

		//set states texture to the particle texture
		states.texture = &_texture;

		//draw all vertices
		target.draw(_vertexArray, states);
	}
	void ParticleNode::addVertex(float worldX, float worldY, float texCoordU, float texCoordV, const sf::Color & color) const
	{
		sf::Vertex vertex;
		vertex.position = sf::Vector2f(worldX, worldY);
		vertex.texCoords = sf::Vector2f(texCoordU, texCoordV);
		vertex.color = color;

		_vertexArray.append(vertex);
	}
	void ParticleNode::computeVertices() const
	{
		sf::Vector2f size(_texture.getSize());
		sf::Vector2f half = size / 2.f;

		//clear and then fill the vertex array
		_vertexArray.clear();
		for(const Particle& p : _particles)
		{
			sf::Vector2f pos = p.position;
			sf::Color color = p.color;

			float ratio = p.lifetime.asSeconds() / TABLE.at(_type).lifetime.asSeconds();
			color.a = static_cast<sf::Uint8>(255 * std::max(ratio, 0.f));//set the a value of color based on the ratio or 0 if it less than that

			//add each vertex that is a corner of the particle
			addVertex(pos.x - half.x, pos.y - half.y, 0.f, 0.f, color);
			addVertex(pos.x + half.x, pos.y - half.y, size.x, 0.f, color);
			addVertex(pos.x + half.x, pos.y + half.y, size.x, size.y, color);
			addVertex(pos.x - half.x, pos.y + half.y, 0.f, size.y, color);
		}
	}
}
