/****************************************************************************************
 * @file        debug_transport.h
 * @author      Sarath S
 * @date        2025-08-07
 * @version     1.0
 * @brief       Debug transport abstraction layer
 *
 * @details
 * This file defines the transport abstraction used by the debug framework.
 * A transport represents the physical or logical interface used to output
 * debug data (UART, USB CDC, RTT, etc.).
 *
 * The abstraction allows the debug core to remain independent of the
 * underlying communication medium by using a function pointer based
 * operations table.
 *
 * @contact     elektronikaembedded@gmail.com
 * @website     https://elektronikaembedded.wordpress.com
 ****************************************************************************************/

#ifndef DEBUG_TRANSPORT_H
#define DEBUG_TRANSPORT_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************** Header include files ************************************/
#include "common.h"

/****************************** Type definitions ****************************************/

/* Forward declaration of debug transport HAL structure */
typedef struct debug_transport_hal debug_transport_hal_t;

/*!
 * \brief Debug transport operations structure
 *
 * This structure defines the set of operations that must be implemented
 * by a debug transport backend (UART, USB CDC, RTT, etc.).
 */
typedef struct
{
    int (*init)(void);                                /*!< Initialize transport */
    int (*deinit)(void);                              /*!< Deinitialize transport */
    int (*write)(const uint8_t *data, size_t len);   /*!< Write data to transport */
} debug_transport_ops_t;

/*!
 * \brief Debug transport HAL structure
 *
 * Holds the transport operations table used internally by the debug core.
 */
struct debug_transport_hal
{
    debug_transport_ops_t debug_transport_ops;        /*!< Transport operations */
};

/****************************** Function declarations ************************************/

/*!----------------------------------------------------------------------------
 * \brief           Initialize debug transport
 * \param[in]       transport   Pointer to debug_transport_hal_t
 * \param[out]      None
 * \param[in/out]   None
 * \return          0 on success, -1 on error
 *
 * \note
 * The actual transport (UART, USB CDC, RTT, etc.) is selected at
 * compile time via configuration options in config.h.
 *---------------------------------------------------------------------------*/
int debug_transport_init(debug_transport_hal_t *transport);

/*!----------------------------------------------------------------------------
 * \brief           Deinitialize debug transport
 * \param[in]       transport   Pointer to debug_transport_hal_t
 * \param[out]      None
 * \param[in/out]   None
 * \return          0 on success, -1 on error
 *
 * \note
 * Calls the selected transport's deinit function if available.
 *---------------------------------------------------------------------------*/
int debug_transport_deinit(debug_transport_hal_t *transport);

#ifdef __cplusplus
}
#endif

#endif /* DEBUG_TRANSPORT_H */

/****************************** End of file *********************************************/
