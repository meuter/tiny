#ifndef __TINY_CORE_ENGINE_H__
#define __TINY_CORE_ENGINE_H__

#include <functional>
#include <chrono>
#include "Inputs.h"
#include "types.h"
#include "../rendering/window.h"

namespace tiny { namespace core {

	class Engine
	{
	public:
		using InputsCallback = std::function<void(Engine &engine, Inputs &inputs)>;
		using UpdateCallback = std::function<void(Engine &engine, sec dt)>;
		using RenderCallback = std::function<void(Engine &engine)>;

		Engine(rendering::Window && window);
		Engine(Engine &&engine) = default;
		Engine(const Engine &other) = delete;
		virtual ~Engine();

		Engine &operator=(const Engine &other) = delete;

		void onInputs(InputsCallback inputsCallback);
		void onUpdate(UpdateCallback updateCallback);
		void onRender(RenderCallback renderCallback);

		void start();
		void stop();

	protected:
		void run();
		bool isRunning() ;
		void input();
		void update(sec dt);
		void render();

	private:
		rendering::Window mWindow;
		Inputs mInputs;
		bool mIsRunning;
		InputsCallback mInputsCallback;
		UpdateCallback mUpdateCallback;
		RenderCallback mRenderCallback;
	};


}}

#endif