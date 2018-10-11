///**
//* @file
//* @author Jeff Ottar-
//* @version 1.0
//*
//*
//* @section DESCRIPTION
//* <  >
//*
//*
//* @section LICENSE
//*
//*
//* Copyright 2017
//* Permission to use, copy, modify, and/or distribute this software for
//* any purpose with or without fee is hereby granted, provided that the
//* above copyright notice and this permission notice appear in all copies.
//*
//* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
//* WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
//* MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
//* ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
//* WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
//* ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
//* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
//*
//* @section Academic Integrity
//* I certify that this work is solely my own and complies with
//* NBCC Academic Integrity Policy (policy 1111)
//*/
//#include "Game.h"
//#include <string>
//
//namespace GEX {
//
//	Game::Game()
//		: _window(sf::VideoMode(600, 400), "Killer Planes"),
//		statisticsNumFrames(0),
//		_statisticsText(),
//		_statisticsUpdateTime(sf::Time::Zero),
//		_world(_window)
//	{
//		if (!_font.loadFromFile("Media/Media/Sansation.ttf"))
//		{
//			//error handling
//		}
//		else {
//			_font.loadFromFile("Media/Media/Sansation.ttf");
//		}
//		_statisticsText.setFont(_font);
//		_statisticsText.setPosition(5.f, 5.f);
//		_statisticsText.setCharacterSize(15.0f);
//		_statisticsText.setString("Frames / Second = \nTime / Update =");
//	}
//
//	
//
//	void Game::run()
//	{
//		const sf::Time SPF = sf::seconds(1.0f / 60.f);//seconds per frame for 60fps
//
//		sf::Clock clock;
//		sf::Time timeSinceLastUpdate = sf::Time::Zero;
//
//		while (_window.isOpen())
//		{
//			processInput();
//			timeSinceLastUpdate = clock.restart();
//
//			while (timeSinceLastUpdate > SPF)//this loop catches the time if it is above spf and catches th update up to it
//			{
//				update(SPF);
//				updateStats(SPF);
//				timeSinceLastUpdate -= SPF;
//			}
//
//			updateStats(timeSinceLastUpdate);
//			update(timeSinceLastUpdate);//update remaining time
//
//
//			render();
//		}
//	}
//
//	void Game::processInput()
//	{
//		CommandQueue& commands = _world.getCommandQueue();
//		//get user input
//		sf::Event event;
//		while (_window.pollEvent(event))//deal with non-realtime events
//		{
//			_player.handleEvent(event, commands);
//
//			if (event.type==sf::Event::Closed) {
//				_window.close();
//			}
//		}
//		_player.handleRealTimeInput(commands);//deal with the realtime events
//	}
//
//	void Game::update(sf::Time timeDelta)
//	{
//		_world.update(timeDelta);
//	}
//
//	void Game::render()
//	{
//		_window.clear();
//		_world.draw();
//
//		_window.setView(_window.getDefaultView());//reset the view to the default
//		_window.draw(_statisticsText);
//		_window.display();
//	}
//
//	void Game::updateStats(sf::Time deltaTime)
//	{
//		_statisticsUpdateTime += deltaTime;
//		statisticsNumFrames += 1;
//		if (_statisticsUpdateTime > sf::seconds(1))
//		{
//			_statisticsText.setString("Frames / Second = " + std::to_string(statisticsNumFrames) + "\n"
//				+ "Time / Update = " + std::to_string(_statisticsUpdateTime.asMicroseconds() / statisticsNumFrames) + "us");
//
//			_statisticsUpdateTime -= sf::seconds(1);
//			statisticsNumFrames = 0;
//		}
//	}
//
//
//}