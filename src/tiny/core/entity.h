#ifndef __TINY_CORE_ENTITY_H__
#define __TINY_CORE_ENTITY_H__

#include "types.h"

namespace tiny { namespace core {

	class Game;

	class Entity
	{
	public:
		Entity() = default;
		Entity(Entity &&other) = default;
		Entity(const Entity &other) = delete;

		virtual ~Entity() = default;

		Entity &operator=(Entity &&entity) = default;
		Entity &operator=(const Entity &entity) = delete;

		template<typename T>
		void add(T &&t) { mComponents.emplace_back(new Wrapper<T>(std::move(t))); }

		void init()                            { for (auto &component: mComponents) component->init(); }
		void render()                          { for (auto &component: mComponents) component->render(); }
		void update(Game &game, sec t, sec dt) { for (auto &component: mComponents) component->update(game, t, dt); }

	private:	
		struct Component
		{
			virtual ~Component() = default;
			virtual void init() = 0;
			virtual void render() = 0;
			virtual void update(Game &game, sec t, sec dt) = 0;
		};

		template <typename T>
		struct Wrapper : public Component
		{
			Wrapper(T &&data) : data(std::move(data)) {}
			virtual void init()                            { data.init();   }
			virtual void render()                          { data.render(); }
			virtual void update(Game &game, sec t, sec dt) { data.update(game, t,dt); }	
			T data;
		};

		std::vector<std::unique_ptr<Component>> mComponents;
	};

}}

#endif