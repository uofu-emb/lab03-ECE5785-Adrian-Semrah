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
#define TEST_RUNNER_PRIORITY ( tskIDLE_PRIORITY + 5UL )

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

    BaseType_t left_status = xTaskCreate(thread_lock, "test thread a",
                SIDE_TASK_STACK_SIZE, (void *)&deadlock_semaphores_a, SIDE_TASK_PRIORITY, &test_thread_a);
    BaseType_t right_status = xTaskCreate(thread_lock, "test thread b",
                SIDE_TASK_STACK_SIZE, (void *)&deadlock_semaphores_b, SIDE_TASK_PRIORITY, &test_thread_b);            

    vTaskDelay(5000);

    TEST_ASSERT_EQUAL_INT(uxSemaphoreGetCount(semaphore_a), 0);
    TEST_ASSERT_EQUAL_INT(uxSemaphoreGetCount(semaphore_b), 0);

    vTaskDelete(test_thread_a);
    vTaskDelete(test_thread_b);
}

void test_orphan()
{
    SemaphoreHandle_t semaphore_a;
    TaskHandle_t test_thread;

    semaphore_a = xSemaphoreCreateCounting(1, 1);
    
    int counter = 0;

    orphaned_t orphan_semaphore = {semaphore_a, &counter};     

    xTaskCreate(thread_orphaned, "test thread a",
                SIDE_TASK_STACK_SIZE, &orphan_semaphore, SIDE_TASK_PRIORITY, &test_thread);

    vTaskDelay(5000);

    vTaskSuspend(test_thread);

    TEST_ASSERT_EQUAL_INT(uxSemaphoreGetCount(semaphore_a), 0);
    TEST_ASSERT_EQUAL_INT(counter, 1);

    vTaskDelete(test_thread);

    xTaskCreate(thread_orphaned, "test thread a",
            SIDE_TASK_STACK_SIZE, &orphan_semaphore, SIDE_TASK_PRIORITY, &test_thread);

    vTaskDelay(5000);

    TEST_ASSERT_EQUAL_INT(uxSemaphoreGetCount(semaphore_a), 0);
    TEST_ASSERT_EQUAL_INT(counter, 1);
    TEST_ASSERT_TRUE_MESSAGE(eTaskGetState(test_thread) == eBlocked, "Thread A did not block corretly");

    vTaskDelete(test_thread);
}

void test_orphan_fixed()
{
    SemaphoreHandle_t semaphore_a;
    TaskHandle_t test_thread;

    semaphore_a = xSemaphoreCreateCounting(1, 1);
    
    int counter = 0;

    orphaned_t orphan_semaphore = {semaphore_a, &counter};     

    xTaskCreate(thread_orphaned_fixed, "test thread a",
                SIDE_TASK_STACK_SIZE, &orphan_semaphore, SIDE_TASK_PRIORITY, &test_thread);
    vTaskDelay(1000 * portTICK_PERIOD_MS);

    vTaskSuspend(test_thread);

    TEST_ASSERT_EQUAL_INT(uxSemaphoreGetCount(semaphore_a), 1);
    TEST_ASSERT_TRUE_MESSAGE(counter > 3, "Counter is not incrementing."); //Means that the count is passing the deadlock point.
    
    vTaskDelete(test_thread);
}


void runner_thread(__unused void *args)
{
    //for (;;) {
        printf("Starting Tests\n");

        UNITY_BEGIN();
        RUN_TEST(test_big_increment_print);
        RUN_TEST(test_increment_print);
        RUN_TEST(test_deadlock);
        RUN_TEST(test_orphan);
        RUN_TEST(test_orphan_fixed);

        UNITY_END();
        //sleep_ms(10000);
    //}
}

int main (void)
{
    stdio_init_all();
    sleep_ms(5000);
    
    xTaskCreate(runner_thread, "TestRunner",
                configMINIMAL_STACK_SIZE, NULL, TEST_RUNNER_PRIORITY, NULL);
    vTaskStartScheduler();

    return 0;
}