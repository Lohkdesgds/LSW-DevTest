#pragma once

// C
#define _CRT_RAND_S
#include <allegro5/allegro5.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_physfs.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>

namespace LSW {
	namespace v5 {
		
		// if initialized now or once successfully, true.
		inline bool lsw_init()					{ if (!al_is_system_installed())				return al_init();													return true; }
		// if initialized now or once successfully, true.
		inline bool lsw_init_acodec_addon()		{ if (!al_is_acodec_addon_initialized()) {		return lsw_init() && al_init_acodec_addon(); }						return true; }
		// if initialized now or once successfully, true.
		inline bool lsw_init_font_addon()		{ if (!al_is_font_addon_initialized()) {		return lsw_init() && al_init_font_addon(); }						return true; }
		// if initialized now or once successfully, true.
		inline bool lsw_init_image_addon()		{ if (!al_is_image_addon_initialized()) {		return lsw_init() && al_init_image_addon(); }						return true; }
		// if initialized now or once successfully, true.
		inline bool lsw_init_primitives_addon()	{ if (!al_is_primitives_addon_initialized()) {	return lsw_init() && al_init_primitives_addon(); }					return true; }
		// if initialized now or once successfully, true.
		inline bool lsw_init_ttf_addon()		{ if (!al_is_ttf_addon_initialized()) {			return lsw_init() && al_init_ttf_addon(); }							return true; }
		// if initialized now or once successfully, true.
		inline bool lsw_install_keyboard()		{ if (!al_is_keyboard_installed()) {			return lsw_init() && al_install_keyboard(); }						return true; }
		// if initialized now or once successfully, true.
		inline bool lsw_install_mouse()			{ if (!al_is_mouse_installed()) {				return lsw_init() && al_install_mouse(); }							return true; }
		// if initialized now or once successfully, true.
		inline bool lsw_install_audio()			{ if (!al_is_audio_installed()) {				return lsw_init() && al_install_audio() && al_reserve_samples(8); }	return true; }

		// tries to initialize all of them. If one gets an error, it might not initialize the others. TRUE if everyone has initialized with no error.
		inline bool lsw_init_all() {
			return lsw_init() && lsw_init_acodec_addon() && lsw_init_font_addon() && lsw_init_image_addon() && lsw_init_primitives_addon() &&
				lsw_init_ttf_addon() && lsw_install_keyboard() && lsw_install_mouse() && lsw_install_audio();
		}
	}
}