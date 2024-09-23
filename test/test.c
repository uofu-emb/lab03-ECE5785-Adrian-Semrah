#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <unity.h>
#include "unity_config.h"

#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>

#include "threads_functions.h"

void setUp(void) {}

void tearDown(void) {}

void test_big_increment_print()
{
    int counter = 0;

    SemaphoreHandle_t semaphore = xSemaphoreCreateCounting(1, 1);

    big_increment_print(semaphore, counter);
    TEST_ASSERT_TRUE_MESSAGE(counter == 1, "Big increment print failed at 0");

    big_increment_print(semaphore, counter);
    TEST_ASSERT_TRUE_MESSAGE(counter == 3, "Big increment print failed at 1");

    big_increment_print(semaphore, counter);
    TEST_ASSERT_TRUE_MESSAGE(counter == 7, "Big increment print failed at 2");
}

void test_increment_print()
{
    int counter = 0;

    SemaphoreHandle_t semaphore = xSemaphoreCreateCounting(1, 1);

    increment_print(semaphore, counter);
    TEST_ASSERT_TRUE_MESSAGE(counter == 1, "Increment print failed at 0");

    increment_print(semaphore, counter);
    TEST_ASSERT_TRUE_MESSAGE(counter == 2, "Increment print failed at 1");

    increment_print(semaphore, counter);
    TEST_ASSERT_TRUE_MESSAGE(counter == 3, "Increment print failed at 2");
}



int main (void)
{
    stdio_init_all();
    sleep_ms(5000);
    printf("Starting Tests\n");

    UNITY_BEGIN();
    RUN_TEST(test_big_increment_print);
    RUN_TEST(test_increment_print);

    sleep_ms(5000);

    return UNITY_END();
}