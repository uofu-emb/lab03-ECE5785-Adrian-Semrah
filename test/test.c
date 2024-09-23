#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <unity.h>
#include "unity_config.h"



void setUp(void) {}

void tearDown(void) {}

void test_variable_assignment()
{
    int x = 1;
    TEST_ASSERT_TRUE_MESSAGE(x == 1,"Variable assignment failed.");
}

v

int main (void)
{
    stdio_init_all();
    UNITY_BEGIN();
    RUN_TEST(test_variable_assignment);
    RUN_TEST(test_multiplication);
    return UNITY_END();
}