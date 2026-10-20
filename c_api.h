/*
 * server.h
 *
 *  Created on: Oct 2, 2026
 *      Author: sandro
 */

#ifndef C_API_H_
#define C_API_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void (*do_work)(void *object, double t, unsigned long ts);
    void (*set_service_available)(void *object, int b);
    void *object;
    double t;
    unsigned long ts;
} Callbacks;


void c_register_callbacks(const Callbacks *callbacks);
void c_run(double t, unsigned long ts);
void c_service_available(int b);

#ifdef __cplusplus
}
#endif

#endif /* C_API_H_ */
