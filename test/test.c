#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <unity.h>
#include "unity_config.h"

#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>

#include "threads_functions.h"

#define SIDE_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define SIDE_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

void setUp(void) {}

void tearDown(void) {}

void test_big_increment_print()
{
    int counter = 0;

    SemaphoreHandle_t semaphore = xSemaphoreCreateCounting(1, 1);

    big_increment_print(semaphore, &counter);
    printf("Counter: %d\n", counter);
    TEST_ASSERT_TRUE_MESSAGE(counter == 1, "Big increment print failed at 0");

    big_increment_print(semaphore, &counter);
    TEST_ASSERT_TRUE_MESSAGE(counter == 3, "Big increment print failed at 1");

    big_increment_print(semaphore, &counter);
    TEST_ASSERT_TRUE_MESSAGE(counter == 7, "Big increment print failed at 2");
}

void test_increment_print()
{
    int counter = 0;

    SemaphoreHandle_t semaphore = xSemaphoreCreateCounting(1, 1);

    increment_print(semaphore, &counter);
    TEST_ASSERT_TRUE_MESSAGE(counter == 1, "Increment print failed at 0");

    increment_print(semaphore, &counter);
    TEST_ASSERT_TRUE_MESSAGE(counter == 2, "Increment print failed at 1");

    increment_print(semaphore, &counter);
    TEST_ASSERT_TRUE_MESSAGE(counter == 3, "Increment print failed at 2");
}

void test_deadlock()
{
    SemaphoreHandle_t semaphore_a, semaphore_b;
    TaskHandle_t test_thread_a, test_thread_b;

    semaphore_a = xSemaphoreCreateCounting(1, 1);
    semaphore_b = xSemaphoreCreateCounting(1, 1);

    char *thread_a_name = "Test Thread A";
    char *thread_b_name = "Test Thread B";

    semaphores_t deadlock_semaphores_a = {semaphore_a, semaphore_b, thread_a_name};        
    semaphores_t deadlock_semaphores_b = {semaphore_b, semaphore_a, thread_b_name};     

    xTaskCreate(thread_lock, "test thread a",
                SIDE_TASK_STACK_SIZE, &deadlock_semaphores_a, SIDE_TASK_PRIORITY, &test_thread_a);
    xTaskCreate(thread_lock, "test thread b",
                SIDE_TASK_STACK_SIZE, &deadlock_semaphores_b, SIDE_TASK_PRIORITY, &test_thread_b);            

    vTaskStartScheduler();
}

int main (void)
{
    stdio_init_all();
    sleep_ms(5000);
    printf("Starting Tests\n");

    UNITY_BEGIN();
    RUN_TEST(test_big_increment_print);
    RUN_TEST(test_increment_print);
    RUN_TEST(test_deadlock);

    sleep_ms(5000);

    return UNITY_END();
}