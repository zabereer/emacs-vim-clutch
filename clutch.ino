#include <TrinketKeyboard.h>

#include "button_pin.hpp"

namespace
{
	constexpr int PEDAL_pin = 0;
	constexpr int BOARD_LED_pin = 1;
	constexpr int TOGGLE_pin = 2;

	bool vim_mode = true;
}

void setup()
{
	pinMode(BOARD_LED_pin, OUTPUT);
	digitalWrite(BOARD_LED_pin, vim_mode ? HIGH : LOW);
	TrinketKeyboard.begin();
}

void loop()
{
	static button_pin pedal{PEDAL_pin};
	static button_pin toggle{TOGGLE_pin};

	TrinketKeyboard.poll();

	static auto last_toggle_ms = millis();
	if (toggle.pressed())
	{
		vim_mode = !vim_mode;
		digitalWrite(BOARD_LED_pin, vim_mode ? HIGH : LOW);
		auto now_ms = millis();
		// if toggle is pressed twice within a second then start emacs
		if (now_ms - last_toggle_ms < 1000)
			TrinketKeyboard.println("emacs . &; disown");
		else
			last_toggle_ms = now_ms;
	}

	auto current_pedal_state = pedal.get_state();
	if (current_pedal_state == button_pin::state::press)
	{
		digitalWrite(BOARD_LED_pin, vim_mode ? LOW : HIGH);
		if (vim_mode)
		{
			TrinketKeyboard.pressKey(0, KEYCODE_I);
		}
		else
		{
			TrinketKeyboard.pressKey(KEYCODE_MOD_LEFT_CONTROL,
						 KEYCODE_X);
			TrinketKeyboard.pressKey(0, 0);
			TrinketKeyboard.pressKey(KEYCODE_MOD_LEFT_CONTROL,
						 KEYCODE_Q);
		}
		TrinketKeyboard.pressKey(0, 0);
	}
	else if (current_pedal_state == button_pin::state::release)
	{
		if (vim_mode)
		{
			TrinketKeyboard.pressKey(0, KEYCODE_ESC);
		}
		else
		{
			TrinketKeyboard.pressKey(KEYCODE_MOD_LEFT_CONTROL,
						 KEYCODE_X);
			TrinketKeyboard.pressKey(0, 0);
			TrinketKeyboard.pressKey(KEYCODE_MOD_LEFT_CONTROL,
						 KEYCODE_Q);
		}
		TrinketKeyboard.pressKey(0, 0);
		digitalWrite(BOARD_LED_pin, vim_mode ? HIGH : LOW);
	}
}
