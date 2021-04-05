#ifndef BUTTON_PIN_HPP
#define BUTTON_PIN_HPP

/**
 * Helper for detecting button press/release of a push-to-make button
 * with pull-down resistor on pin.
 */
class button_pin
{
	static constexpr decltype(millis()) debounce_delay_ms = 100;

public:
	explicit button_pin(const int pin_with_pull_down)
		: pin{pin_with_pull_down}
	{
		pinMode(pin, INPUT);
	}

	enum class state { same, press, release };

	state get_state()
	{
		auto now_ms = millis();
		auto current = digitalRead(pin);
		if (current == last || now_ms - last_ms < debounce_delay_ms)
		{
			return state::same;
		}
		last_ms = now_ms;
		last = current;
		return current == HIGH ? state::press : state::release;
	}

	bool pressed()
	{
		return get_state() == state::press;
	}

private:
	const int pin;
	decltype(millis()) last_ms = millis();
	decltype(digitalRead(pin)) last = LOW;
};

#endif
