#include "engine.h"
#include "clock.h"
#include <thread>

namespace tiny { namespace core {

static const auto NO_INPUTS_CB = [](Engine &engine, Inputs &inputs){};
static const auto NO_UPDATE_CB = [](Engine &engine, sec dt){};
static const auto NO_RENDER_CB = [](Engine &engine){};

Engine::Engine(rendering::Window && window) 
	: mWindow(std::move(window)), 
	  mIsRunning(false), 
	  mInputsCallback(NO_INPUTS_CB),
	  mUpdateCallback(NO_UPDATE_CB),
	  mRenderCallback(NO_RENDER_CB) {}

Engine::~Engine()
{

}

void Engine::onInputs(InputsCallback inputsCallback)
{
	mInputsCallback = inputsCallback;
}

void Engine::onUpdate(UpdateCallback updateCallback)
{
	mUpdateCallback = updateCallback;
}

void Engine::onRender(RenderCallback renderCallback)
{
	mRenderCallback = renderCallback;
}

void Engine::start() 
{
	if (isRunning())
		return;

		mIsRunning = true; 
		run();
}

void Engine::stop() 
{
	if (!isRunning())
		return;

	mIsRunning = false; 
}

void Engine::run()
{
	static const auto dt = sec(1.0/5000);

	auto clock = Clock();
	auto unprocessedTime = sec(0);

	while (isRunning())
	{
		unprocessedTime += clock.lap();

		while (unprocessedTime >= dt)
		{
			input();
			update(dt);
			unprocessedTime -= dt;			
		}

		render();
	}
}

bool Engine::isRunning() 
{
	return mIsRunning; 
}

void Engine::input()
{
	mInputs.refresh();
	mInputsCallback(*this, mInputs);
}

void Engine::update(sec dt)
{
	mUpdateCallback(*this, dt);
}

void Engine::render()
{
	mRenderCallback(*this);
	mWindow.update();
}


}}