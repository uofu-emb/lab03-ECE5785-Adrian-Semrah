
#ifndef _THREAD_FUNCTIONS_H_
#define _THREAD_FUNCTIONS_H_

#include <stdio.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>

void big_increment_print(SemaphoreHandle_t semaphore, int* counter);

void increment_print(SemaphoreHandle_t semaphore, int* counter);

#endif
