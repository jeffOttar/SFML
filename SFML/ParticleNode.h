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

#pragma once
#include "SceneNode.h"
#include "Particle.h"
#include "DataTables.h"
#include "TextureManager.h"

#include <SFML/Graphics/VertexArray.hpp>

#include <deque>

namespace GEX {
	class ParticleNode :
		public SceneNode
	{
	public:
							ParticleNode(Particle::Type type, const TextureManager& textures);

		void				addParticle(sf::Vector2f position);
		Particle::Type		getParticleType() const;
		unsigned int		getCategory() const override;

	private:
		void				updateCurrent(sf::Time dt, CommandQueue& commands) override;
		void				drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

		void				addVertex(float worldX, float worldY, float texCoordU, float texCoordV, const sf::Color& color) const;
		void				computeVertices() const;

	private:
		std::deque<Particle>	_particles;
		const sf::Texture&		_texture;
		Particle::Type			_type;

		//mutable allows you to make the addvertex and computeVertices const while still changing these objects
		mutable sf::VertexArray	_vertexArray;
		mutable bool			_needsVertexUpdate;

	};

}