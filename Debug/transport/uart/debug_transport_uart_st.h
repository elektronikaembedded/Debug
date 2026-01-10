/**
 * @file      debug_transport_uart_st.h
 * @brief     UART debug transport interface (STM32)
 * @version   1.0.0
 * @date      2026-01-02
 * @author    Sarath S
 *
 * @details
 * This header declares the UART-based debug transport layer used by the
 * debug framework on STM32 platforms.
 *
 * It provides access to a transport operations table implementing the
 * standard debug transport interface, enabling debug output over UART
 * while keeping the debug core transport-agnostic.
 *
 * @par Contact
 * elektronikaembedded@gmail.com
 *
 * @par Website
 * https://elektronikaembedded.wordpress.com
 */

#ifndef DEBUG_TRANSPORT_UART_ST_H
#define DEBUG_TRANSPORT_UART_ST_H

#include "config.h"

#if DEBUG_USE_UART

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 *******************************************************************************/
#include "common.h"
#include "debug_transport.h"    /**< Required for debug_transport_ops_t */

/*******************************************************************************
 * Public Functions
 *******************************************************************************/

/**
 * @brief Get UART debug transport operations (STM32).
 *
 * @return Pointer to the UART transport operations table.
 *
 * @note
 * This function is called by the debug transport selector during
 * framework initialization to obtain function pointers for the
 * UART transport backend.
 */
const debug_transport_ops_t *debug_transport_uart_st_ops(void);

#ifdef __cplusplus
}
#endif

#endif /* DEBUG_USE_UART */
#endif /* DEBUG_TRANSPORT_UART_ST_H */

/*******************************************************************************
 * End of file
 *******************************************************************************/
