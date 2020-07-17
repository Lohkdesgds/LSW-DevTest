#pragma once

#include "..\sprite\sprite.h"
#include "..\block\block.h"
#include "..\text\text.h"

namespace LSW {
	namespace v5 {

		// Button is kinda Block with Text inside (its attributes are a copy of Block's one, same stuff)
		class Button : public Sprite_Base { // how to make them like one? Do I really need to setup all std::functions to use its data instead?
			std::shared_ptr<Sprite_Base> block;
			std::shared_ptr<Sprite_Base> text;

		public:
			using Sprite_Base::set;
			using Sprite_Base::get;
			using Sprite_Base::getRef;

			Button();

			Text* getText();
			Block* getBlock();
		};
	}
}