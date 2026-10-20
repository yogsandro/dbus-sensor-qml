/*
 * c_api.c
 *
 *  Created on: Oct 2, 2026
 *      Author: sandro
 */


#include <stdio.h>
#include "c_api.h"

static Callbacks callbacks;

void c_register_callbacks(const Callbacks *new_callbacks)
{
	printf("in c_register_callbacks()\n");
    callbacks = *new_callbacks;
}

void c_run(double t, unsigned long ts)
{
    if (callbacks.do_work != NULL) {
        callbacks.do_work(callbacks.object, t, ts);
    }
}

void c_service_available(int b)
{
    if (callbacks.set_service_available != NULL) {
        callbacks.set_service_available(callbacks.object, b);
    }
}
