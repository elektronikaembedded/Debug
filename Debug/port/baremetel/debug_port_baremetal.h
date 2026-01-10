/****************************************************************************************
 * @file        debug_port_baremetal.h
 * @author      Sarath S
 * @date        2026-01-02
 * @version     1.0
 * @brief       Bare-metal debug port interface
 *
 * @details
 * This file declares the bare-metal debug port layer for the debug framework.
 *
 * It provides minimal OS abstraction services for systems without an RTOS:
 *   - Locking and unlocking (implemented as no-ops)
 *   - ISR context detection (stub or CPU-specific)
 *   - Timestamp retrieval (stub or system tick based)
 *   - Thread name access (returns static identifiers: "MAIN" or "ISR")
 *
 * Only a single accessor function is exposed, returning the operations table
 * required by the debug core.
 *
 * @note
 * The actual implementation is provided in debug_port_baremetal.c and selected
 * at compile time using DEBUG_USE_BAREMETAL in config.h.
 *
 * @contact     elektronikaembedded@gmail.com
 * @website     https://elektronikaembedded.wordpress.com
 ****************************************************************************************/

#ifndef DEBUG_PORT_BAREMETAL_H
#define DEBUG_PORT_BAREMETAL_H

#include "config.h"

#if DEBUG_USE_BAREMETAL

#ifdef __cplusplus
extern "C" {
#endif

/****************************** Header include files ************************************/
#include "common.h"
#include "debug_port.h"   /*!< Required for debug_port_ops_t */

/****************************** Public Function Declarations ****************************/

/**
 * @brief Get bare-metal debug port operations table
 *
 * @return Pointer to constant bare-metal debug port operations table
 *
 * @details
 * This function is called by the debug framework during initialization to obtain
 * the bare-metal implementation of the debug port operations.
 *
 * @note
 * Intended for use in systems without an operating system.
 */
const debug_port_ops_t *debug_port_baremetal_ops(void);

#ifdef __cplusplus
}
#endif

#endif /* DEBUG_USE_BAREMETAL */
#endif /* DEBUG_PORT_BAREMETAL_H */

/****************************** End of file *********************************************/
