#ifndef __TINY_CORE_ENGINE_H__
#define __TINY_CORE_ENGINE_H__

#include <functional>
#include "Inputs.h"
#include "types.h"
#include "../rendering/window.h"

namespace tiny { namespace core {

	class Engine
	{
	public:
		using Window = rendering::Window;
		using InitCallback   = std::function<void(Engine &engine)>;
		using InputsCallback = std::function<void(Engine &engine)>;
		using UpdateCallback = std::function<void(Engine &engine, sec dt)>;
		using RenderCallback = std::function<void(Engine &engine)>;

		Engine(rendering::Window && window);
		Engine(Engine &&engine) = default;
		Engine(const Engine &other) = delete;
		virtual ~Engine();

		Engine &operator=(const Engine &other) = delete;

		void onInit(InitCallback initCallback);
		void onInputs(InputsCallback inputsCallback);
		void onUpdate(UpdateCallback updateCallback);
		void onRender(RenderCallback renderCallback);

		void start();
		void stop();

		template<typename Game>
		void play(Game &&game)
		{
			onInit  ([&game](Engine &engine)          { game.init(engine); });
			onInputs([&game](Engine &engine)          { game.inputs(engine); });
			onUpdate([&game](Engine &engine, sec dt)  { game.update(engine, dt); });
			onRender([&game](Engine &engine)          { game.render(engine); });
			start();
		}

		inline Window &window() { return mWindow; }
		inline Inputs &inputs() { return mInputs; }

	protected:
		void run();
		bool isRunning();
		void init();
		void input();
		void update(sec dt);
		void render();

	private:
		Window mWindow;
		Inputs mInputs;
		bool mIsRunning;
		InitCallback mInitCallback;
		InputsCallback mInputsCallback;
		UpdateCallback mUpdateCallback;
		RenderCallback mRenderCallback;
	};


}}

#endif