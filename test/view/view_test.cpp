#include "unity.h"

#include "utils.hpp"
#include "view.hpp"

using greenhouse::utils::eps;
using greenhouse::view::View;

void view_returns_eps_if_no_message_test(void)
{
    View<5> view;
    TEST_ASSERT_EQUAL_STRING(eps.c_str(), view.get_message().c_str());
}

void view_returns_first_message_after_adding_one_or_more_test(void)
{
    View<5> view;
    view.add_message("Hello");
    view.add_message("World");

    TEST_ASSERT_EQUAL_STRING(String("Hello").c_str(), view.get_message().c_str());
}

void view_returns_second_message_after_advancing_by_one(void)
{
    View<5> view;
    view.add_message("Hello");
    view.add_message("World");

    view.next();

    TEST_ASSERT_EQUAL_STRING(String("World").c_str(), view.get_message().c_str());
}

void view_returns_eps_if_after_adding_messages_clear_is_called_test(void)
{
    View<5> view;
    view.add_message("Hello");
    view.add_message("World");

    TEST_ASSERT_EQUAL_STRING(String("Hello").c_str(), view.get_message().c_str());

    view.clear();

    TEST_ASSERT_EQUAL_STRING(eps.c_str(), view.get_message().c_str());
}

void view_advances_by_one_if_showcase_disabled_but_next_is_forced_test(void)
{
    View<5> view;
    view.add_message("Hello");
    view.add_message("World");

    TEST_ASSERT_EQUAL_STRING(String("Hello").c_str(), view.get_message().c_str());

    view.disable();
    view.next();

    TEST_ASSERT_EQUAL_STRING(String("Hello").c_str(), view.get_message().c_str());

    view.next(true);

    TEST_ASSERT_EQUAL_STRING(String("World").c_str(), view.get_message().c_str());
}

void view_advances_again_if_slideshow_is_then_enabled_test(void)
{
    View<5> view;
    view.add_message("Hello");
    view.add_message("World");

    TEST_ASSERT_EQUAL_STRING(String("Hello").c_str(), view.get_message().c_str());

    view.disable();
    view.next();

    TEST_ASSERT_EQUAL_STRING(String("Hello").c_str(), view.get_message().c_str());

    view.enable();
    view.next();

    TEST_ASSERT_EQUAL_STRING(String("World").c_str(), view.get_message().c_str());
}

void view_returns_first_message_if_next_called_from_last(void)
{
    View<2> view;
    view.add_message("Hello");
    view.add_message("World");

    view.next();

    TEST_ASSERT_EQUAL_STRING(String("World").c_str(), view.get_message().c_str());

    view.next();

    TEST_ASSERT_EQUAL_STRING(String("Hello").c_str(), view.get_message().c_str());

    view.next();
    view.next();

    TEST_ASSERT_EQUAL_STRING(String("Hello").c_str(), view.get_message().c_str());
}

void view_does_not_add_more_messages_if_full_test()
{
    View<2> view;
    view.add_message("Hello");
    view.add_message("World");
    view.add_message("Awesome!");

    TEST_ASSERT_EQUAL_STRING(String("Hello").c_str(), view.get_message().c_str());

    view.next();

    TEST_ASSERT_EQUAL_STRING(String("World").c_str(), view.get_message().c_str());

    view.next();

    TEST_ASSERT_EQUAL_STRING(String("Hello").c_str(), view.get_message().c_str());
}

void view_insert_new_elements_if_full_and_clear_called_test(void)
{
    View<2> view;
    view.add_message("Hello");
    view.add_message("World");

    TEST_ASSERT_EQUAL_STRING(String("Hello").c_str(), view.get_message().c_str());

    view.clear();

    view.add_message("Awesome!");

    TEST_ASSERT_EQUAL_STRING(String("Awesome!").c_str(), view.get_message().c_str());
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(view_returns_eps_if_no_message_test);
    RUN_TEST(view_returns_first_message_after_adding_one_or_more_test);
    RUN_TEST(view_returns_second_message_after_advancing_by_one);
    RUN_TEST(view_returns_eps_if_after_adding_messages_clear_is_called_test);
    RUN_TEST(view_advances_by_one_if_showcase_disabled_but_next_is_forced_test);
    RUN_TEST(view_advances_again_if_slideshow_is_then_enabled_test);
    RUN_TEST(view_returns_first_message_if_next_called_from_last);
    RUN_TEST(view_does_not_add_more_messages_if_full_test);
    RUN_TEST(view_insert_new_elements_if_full_and_clear_called_test);

    UNITY_END();
}
