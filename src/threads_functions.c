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

    printf("Attempting to take Semaphore A in Thread: %s\n", semaphore_deadlock.thread_name);
    xSemaphoreTake(semaphore_deadlock.a, portMAX_DELAY);
    {   
        printf("Took Semaphore A in Thread: %s\n", semaphore_deadlock.thread_name);
        vTaskDelay(1000);
        
        printf("Attempting to take Semaphore B in Thread: %s\n", semaphore_deadlock.thread_name);
        xSemaphoreTake(semaphore_deadlock.b, portMAX_DELAY);
            {
                printf("Took Semaphore B in Thread: %s\n", semaphore_deadlock.thread_name);
            }
        xSemaphoreGive(semaphore_deadlock.b);
        printf("Released Semaphore B in Thread: %s\n", semaphore_deadlock.thread_name);

        vTaskDelay(1000);
    }
    xSemaphoreGive(semaphore_deadlock.a);
    printf("Released Semaphore A in Thread: %s\n", semaphore_deadlock.thread_name);
}

void thread_orphaned(void *params)
{
    orphaned_t orphaned = *((orphaned_t*) params);

    while (1) {
        xSemaphoreTake(orphaned.a, portMAX_DELAY);
        (*orphaned.counter)++;
        if ((*orphaned.counter) % 2) {
            continue;
        }
        printf("Count %d\n", (*orphaned.counter));
        xSemaphoreGive(orphaned.a);
    }
}

void thread_orphaned_fixed(void *params)
{
    orphaned_t orphaned = *((orphaned_t*) params);

    int loop_count = 0;

    while (loop_count < 10) {
        xSemaphoreTake(orphaned.a, portMAX_DELAY);
        (*orphaned.counter)++;

        if ((*orphaned.counter) % 2) {
            xSemaphoreGive(orphaned.a);
            loop_count++;
            continue;
        }

        printf("Count %d\n", (*orphaned.counter));
        loop_count++;
        xSemaphoreGive(orphaned.a);
    }

    while(1){
        vTaskDelay(1000);
    }
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




