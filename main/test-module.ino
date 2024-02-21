/*
 *	test function library
 *
 *
 *
 */
#include "test-module.h"

void test_init(void)
{
    pinMode(28, INPUT);
    pinMode(29, INPUT);
    pinMode(30, INPUT);
}

uint32_t FreeMem()
{ // for Teensy 3.0
    uint32_t stackTop;
    uint32_t heapTop;

    // current position of the stack.
    stackTop = (uint32_t)&stackTop;

    // current position of heap.
    void *hTop = malloc(1);
    heapTop = (uint32_t)hTop;
    free(hTop);

    // The difference is (approximately) the free, available ram.
    return stackTop - heapTop;
}