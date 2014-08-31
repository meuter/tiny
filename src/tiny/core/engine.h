#ifndef __TINY_CORE_ENGINE_H__
#define __TINY_CORE_ENGINE_H__

#include <functional>
#include "Inputs.h"
#include "types.h"
#include "../rendering/window.h"

namespace tiny { namespace core {

	class Game;

	class Engine
	{
	public:
		using Window = rendering::Window;

		Engine(rendering::Window && window);
		Engine(Engine &&engine) = default;
		Engine(const Engine &other) = delete;
		virtual ~Engine();

		Engine &operator=(const Engine &other) = delete;

		void play(Game &&game);

		inline sec     delta()  const { return sec(1.0/5000.0); }
		inline Window &window()       { return mWindow; }
		inline Inputs &inputs()       { return mInputs; }

	private:
		Window mWindow;
		Inputs mInputs;
	};


}}

#endif