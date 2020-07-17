#include "button.h"

namespace LSW {
	namespace v5 {
		Button::Button()
		{
			SuperResource<Sprite_Base> sprites;
			std::string sign = Tools::generateRandomUniqueStringN();
			std::string my_id;

			std::shared_ptr<Sprite_Base> thus;

			sprites.lock();
			for (auto& i : sprites) {
				if (&(*i.data()) == (void*)this) {
					my_id = i.getID();
					thus = i.data();
					break;
				}
			}
			sprites.unlock();

			block = sprites.customLoad(my_id + "_b&" + sign + "+", [](Sprite_Base*& b) {return (b = new Block()); });
			text = sprites.customLoad(my_id + "_t&" + sign + "+", [](Sprite_Base*& b) {return (b = new Text()); });

			getText()->set(text::e_sprite_ptr::FOLLOWING, block);
			getText()->set(sprite::e_integer::COLLISION_MODE, static_cast<int>(sprite::e_collision_mode_cast::COLLISION_NONE));

			reinterpret_cast<Sprite_Base*>(getBlock())->twinUpAttributes(reinterpret_cast<Sprite_Base*>(this)->getAttributes()); // b() attributes now are this attributes (spritewise)
		}

		Text* Button::getText()
		{
			return (Text*)&(*text);
		}

		Block* Button::getBlock()
		{
			return (Block*)&(*block);
		}
	}
}