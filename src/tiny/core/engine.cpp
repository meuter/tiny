#include "Engine.h"

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

	const auto FRAME_CAP = 500;
	const auto dt = std::chrono::duration<double>(1.0/FRAME_CAP);

	auto currentTime = std::chrono::high_resolution_clock::now();
	auto unprocessedTime = std::chrono::duration<double>(0.0);

	while (isRunning())
	{
		auto newTime = std::chrono::high_resolution_clock::now();
		auto frameTime = std::chrono::duration_cast<std::chrono::duration<double> >(newTime - currentTime);
		currentTime = newTime;

		unprocessedTime += frameTime;

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