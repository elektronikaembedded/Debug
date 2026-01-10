/**
 * @file      debug_transport.h
 * @brief     Debug transport abstraction layer
 * @version   1.0.0
 * @date      2025-08-07
 * @author    Sarath S
 *
 * @details
 * This header defines the transport abstraction used by the debug framework.
 *
 * A transport represents the physical or logical interface used to output
 * debug data, such as:
 *   - UART
 *   - USB CDC
 *   - RTT (future extension)
 *
 * The abstraction enables the debug core to remain independent of the
 * underlying communication medium by using a function-pointer-based
 * operations table.
 *
 * @par Contact
 * elektronikaembedded@gmail.com
 *
 * @par Website
 * https://elektronikaembedded.wordpress.com
 */

#ifndef DEBUG_TRANSPORT_H
#define DEBUG_TRANSPORT_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 *******************************************************************************/
#include "common.h"

/*******************************************************************************
 * Public Types
 *******************************************************************************/

/* Forward declaration of debug transport HAL structure */
typedef struct debug_transport_hal debug_transport_hal_t;

/**
 * @brief Debug transport operations interface
 *
 * This structure defines the set of operations that must be implemented
 * by a debug transport backend (e.g., UART, USB CDC, RTT).
 *
 * Each backend provides an instance of this structure to the debug core.
 */
typedef struct
{
    int (*init)(void);                             /**< Initialize transport */
    int (*deinit)(void);                           /**< Deinitialize transport */
    int (*write)(const uint8_t *data,
                 size_t len);                      /**< Write data to transport */
} debug_transport_ops_t;

/**
 * @brief Debug transport HAL structure
 *
 * Holds the transport operations table used internally by the debug core.
 * The concrete implementation is selected at compile time.
 */
struct debug_transport_hal
{
    const debug_transport_ops_t *ops;     /**< Transport operations */
};

/*******************************************************************************
 * Public Functions
 *******************************************************************************/

/**
 * @brief Initialize the debug transport layer.
 *
 * @param[in,out] transport Pointer to a debug transport HAL instance.
 *
 * @retval 0   Transport initialized successfully.
 * @retval -1  Invalid transport pointer or initialization failure.
 *
 * @note
 * The actual transport backend (UART, USB CDC, RTT, etc.) is selected
 * at compile time using configuration options in config.h.
 */
int debug_transport_init(debug_transport_hal_t *transport);

/**
 * @brief Deinitialize the debug transport layer.
 *
 * @param[in,out] transport Pointer to a debug transport HAL instance.
 *
 * @retval 0   Transport deinitialized successfully.
 * @retval -1  Invalid transport pointer or deinitialization failure.
 *
 * @note
 * Calls the selected transport's deinit() function if it is implemented.
 * If no deinit function is provided, this call returns success.
 */
int debug_transport_deinit(debug_transport_hal_t *transport);

#ifdef __cplusplus
}
#endif

#endif /* DEBUG_TRANSPORT_H */

/*******************************************************************************
 * End of file
 *******************************************************************************/
