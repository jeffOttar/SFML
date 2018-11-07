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
#include "PostEffect.h"

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shader.hpp>

#include <array>
#include <map>

namespace GEX {
	class BloomEffect : public PostEffect
	{
	public:
		enum class Shaders
		{
			BrightnessPass,
			DownSamplePass,
			GaussianBlurPass,
			AddPass,
		};

	private:
		typedef std::array<sf::RenderTexture, 2> RenderTextureArray; //array of rendertextures with size of 2

	public:
		BloomEffect();

		void					apply(const sf::RenderTexture& input, sf::RenderTarget& output) override;

	
	private:
		void					prepareTextures(sf::Vector2u size);//u is unsigned int

		void					filterBright(const sf::RenderTexture& input, sf::RenderTexture& output);

		void					blurMultipass(RenderTextureArray& renderTextures);

		void					blur(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f offsetFactor);

		void					downsample(const sf::RenderTexture& input, sf::RenderTexture& output);

		void					add(const sf::RenderTexture& source, const sf::RenderTexture& bloom, sf::RenderTarget& target);


	private:
		std::map<Shaders, std::unique_ptr<sf::Shader>> shaders_;

		sf::RenderTexture		brightnessTexture_;
		RenderTextureArray		firstPassTexture_;
		RenderTextureArray		secondPassTexture_;
	};

}