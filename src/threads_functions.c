#include "threads_functions.h"

#include <stdio.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <pico/cyw43_arch.h>

void big_increment_print(SemaphoreHandle_t semaphore, int counter)
{
    xSemaphoreTake(semaphore, portMAX_DELAY);
    {
        counter += counter + 1;
        printf("hello world from %s! Count %d\n", "thread", counter);
    }
    xSemaphoreGive(semaphore);
}

void increment_print(SemaphoreHandle_t semaphore, int counter)
{
    xSemaphoreTake(semaphore, portMAX_DELAY);
    {
        printf("hello world from %s! Count %d\n", "main", counter++);
    }
    xSemaphoreGive(semaphore);
}



