/****************************************************************************************
 * @file        debug_transport_uart_st.h
 * @author      Sarath S
 * @date        2026-01-02
 * @version     1.0
 * @brief       STM32 UART debug transport interface
 *
 * @details
 * Declares the UART-based debug transport layer used by the debug framework.
 * Provides a function to retrieve the transport operations table for sending
 * debug data over UART. This allows the debug core to remain transport-agnostic.
 *
 * @contact     elektronikaembedded@gmail.com
 * @website     https://elektronikaembedded.wordpress.com
 ****************************************************************************************/

#ifndef DEBUG_TRANSPORT_UART_ST_H
#define DEBUG_TRANSPORT_UART_ST_H

#include "config.h"

#if DEBUG_USE_UART

#ifdef __cplusplus
extern "C" {
#endif

/****************************** Header include files ************************************/
#include "common.h"
#include "debug_transport.h"    /*!< Required for debug_transport_ops_t */

/****************************** Function prototypes ************************************/

/*!----------------------------------------------------------------------------
 * \brief           Get UART debug transport operations
 * \return          Pointer to UART transport operations table
 *
 * \note
 * This function is called by the debug core during initialization to obtain
 * function pointers for the UART transport operations (init, deinit, write).
 * Intended for STM32 platforms.
 *---------------------------------------------------------------------------*/
const debug_transport_ops_t *debug_transport_uart_st_ops(void);

#ifdef __cplusplus
}
#endif

#endif /* DEBUG_USE_UART */
#endif /* DEBUG_TRANSPORT_UART_ST_H */

/****************************** End of file *********************************************/
