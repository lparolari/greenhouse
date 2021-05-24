#include <unity.h>
#include "delay.hpp"

void delay_is_not_fired_after_creation_test()
{
    greenhouse::delay::Delay<0> d1;
    greenhouse::delay::Delay<3000> d2;

    TEST_ASSERT_FALSE(d1.is_fired())
    TEST_ASSERT_FALSE(d2.is_fired())
}

void delay_is_fired_if_forwarded_by_at_least_delayamount_test()
{
    greenhouse::delay::Delay<3000> d;

    d.tick(1000);
    TEST_ASSERT_FALSE(d.is_fired());

    d.tick(3000);
    TEST_ASSERT_FALSE(d.is_fired());

    d.tick(3001);
    TEST_ASSERT_TRUE(d.is_fired())
}

void delay_is_fired_if_forwarded_by_at_least_delayamount_even_if_cleared_test()
{
    greenhouse::delay::Delay<3000> d;

    d.tick(3500);
    TEST_ASSERT_TRUE(d.is_fired());

    d.clear();
    TEST_ASSERT_FALSE(d.is_fired());

    d.tick(9000);
    TEST_ASSERT_TRUE(d.is_fired());
}

void delay_clear_do_not_reset_previous_time()
{
    greenhouse::delay::Delay<3000> d;

    d.tick(3500);
    TEST_ASSERT_TRUE(d.is_fired());

    d.clear();
    TEST_ASSERT_FALSE(d.is_fired());

    // We do not expect `d.clear()` reset delay status such that it computes
    // 4000 - 0 > 3000 and fires

    d.tick(4000);
    TEST_ASSERT_FALSE(d.is_fired());
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(delay_is_not_fired_after_creation_test);
    RUN_TEST(delay_is_fired_if_forwarded_by_at_least_delayamount_test);
    RUN_TEST(delay_is_fired_if_forwarded_by_at_least_delayamount_even_if_cleared_test);
    RUN_TEST(delay_clear_do_not_reset_previous_time);

    UNITY_END();
}
