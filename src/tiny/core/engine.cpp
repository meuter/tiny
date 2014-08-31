#include "Engine.h"



namespace tiny { namespace core {

static const auto NO_INPUTS_CB = [](Engine &engine, Inputs &inputs){};
static const auto NO_UPDATE_CB = [](Engine &engine, double dt){};
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
	while (isRunning())
	{
		input();
		update();
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

void Engine::update()
{
	mUpdateCallback(*this, 0.0);
}

void Engine::render()
{
	mRenderCallback(*this);
	mWindow.update();
}


}}