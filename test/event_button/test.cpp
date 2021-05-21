#include <Arduino.h>
#include <Wire.h>
#include <unity.h>

#include "button.hpp"

class MockInputSequence
{
private:
    // We need to set `i` to -1 because in a real scenario we first read
    // values and then we check for `pressed()`. In this case first reading
    // means setting `i = 1` if it was initialized with `0`.
    int i = -1;
    int values[12] = {LOW, LOW, HIGH, HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW};

public:
    void begin() {}

    void tick(uint64_t ms = 0) {}

    void read()
    {
        i = (i + 1) % 10;
    }

    bool pressed() const
    {
        return values[i] == HIGH;
    }
};

void button_is_not_pressed_nor_released_if_value_not_forwarded_test(void)
{
    greenhouse::button::_EventButton<MockInputSequence> event_button;
    event_button.begin();

    TEST_ASSERT_FALSE(event_button.pressed());
    TEST_ASSERT_FALSE(event_button.released());
}

void button_fires_correct_event()
{
    // events ground truth
    bool pressed[10] = {false, false, true, true, false, false, false, false, false, false};
    bool released[10] = {false, false, false, false, false, false, true, true, false, false};

    greenhouse::button::_EventButton<MockInputSequence> event_button;
    event_button.begin();

    for (int i = 0; i < 10; i++)
    {
        event_button.read();
        event_button.tick();

        TEST_ASSERT_EQUAL(pressed[i], event_button.pressed());
        TEST_ASSERT_EQUAL(released[i], event_button.released());
    }
}

void setup()
{
    // Wait if Serial.DTR/RTS not supported before UNITY_BEING()
    delay(2000);

    UNITY_BEGIN();

    RUN_TEST(button_is_not_pressed_nor_released_if_value_not_forwarded_test);
    RUN_TEST(button_fires_correct_event);

    UNITY_END();
}

void loop()
{
}
