/*******************************************************************************
 * @file    debug_transport.h
 * @brief   Debug transport interface
 *******************************************************************************/

#ifndef DEBUG_TRANSPORT_H
#define DEBUG_TRANSPORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct debug_transport debug_transport_t;

/* Transport operations */
typedef struct
{
    int (*init)(void);
    int (*deinit)(void);
    int (*write)(const uint8_t *data, size_t len);
} debug_transport_ops_t;

/* Transport handle */
struct debug_transport
{
    const debug_transport_ops_t *ops;
};

int debug_transport_init(debug_transport_t *trns);
#ifdef __cplusplus
}
#endif

#endif /* DEBUG_TRANSPORT_H */
