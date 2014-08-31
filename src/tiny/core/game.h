#ifndef __TINY_CORE_GAME_H__
#define __TINY_CORE_GAME_H__

#include "engine.h"

namespace tiny { namespace core {

	class Game
	{
	public:
		Game() {}
		Game(const Game &other) = delete;
		Game(Game &&other) = default;
		virtual ~Game() {}

		Game &operator=(const Game &other) = delete;
		Game &operator=(Game &&other) = default;

		virtual void init(Engine &engine)    {}
		virtual void inputs(Engine &engine)  {}
		virtual void update(Engine &engine)  {}
		virtual void render(Engine &engine)  {}
	};

}}

#endif