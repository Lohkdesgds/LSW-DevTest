#pragma once

#include "..\sprite\sprite.h"

namespace LSW {
	namespace v5 {
		class Entity : public Sprite_Base {
			ALLEGRO_BITMAP* bmp = nullptr;

			void draw_self();
		public:
			Entity();
			Entity(Entity& o) : Sprite_Base(o) {}
			~Entity();

			void load(const std::string);
		};
	}
}