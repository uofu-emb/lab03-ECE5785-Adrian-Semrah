#include "threads_functions.h"

#include <stdio.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <pico/cyw43_arch.h>



void thread_lock(void *params)
{
    semaphores_t semaphore_deadlock = *((semaphores_t*) params);

    printf("Attempting to take Semaphore A in Thread: %s", semaphore_deadlock.thread_name);
    xSemaphoreTake(semaphore_deadlock.a, portMAX_DELAY);
    {   
        printf("Took Semaphore A in Thread: %s", semaphore_deadlock.thread_name);

        printf("Attempting to take Semaphore B in Thread: %s", semaphore_deadlock.thread_name);
        xSemaphoreTake(semaphore_deadlock.b, portMAX_DELAY);
            {
                printf("Took Semaphore B in Thread: %s", semaphore_deadlock.thread_name);
                vTaskDelay(1000);
            }
        xSemaphoreGive(semaphore_deadlock.b);
        printf("Released Semaphore B in Thread: %s", semaphore_deadlock.thread_name);

        vTaskDelay(1000);
    }
    xSemaphoreGive(semaphore_deadlock.a);
    printf("Released Semaphore A in Thread: %s", semaphore_deadlock.thread_name);
}

void big_increment_print(SemaphoreHandle_t semaphore, int* counter)
{
    xSemaphoreTake(semaphore, portMAX_DELAY);
    {
        *counter += *counter + 1;
        printf("hello world from %s! Count %d\n", "thread", *counter);
    }
    xSemaphoreGive(semaphore);
}

void increment_print(SemaphoreHandle_t semaphore, int* counter)
{
    xSemaphoreTake(semaphore, portMAX_DELAY);
    {
        printf("hello world from %s! Count %d\n", "main", (*counter)++);
    }
    xSemaphoreGive(semaphore);
}



