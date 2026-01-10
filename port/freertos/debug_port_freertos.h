/****************************************************************************************
 * @file        debug_port_baremetal.h
 * @author      Sarath S
 * @date        2026-01-02
 * @version     1.0
 * @brief       Bare-metal debug port interface
 *
 * @details
 * Declares the bare-metal debug port layer for the debug framework.
 * This implementation provides minimal OS abstraction services suitable
 * for systems without an RTOS.
 *
 * Features:
 *   - Locking / unlocking        : No-op
 *   - ISR detection              : Stub or architecture-specific
 *   - Timestamp retrieval        : Stub or system tick based
 *   - Thread name access         : Static string ("MAIN" or "ISR")
 *
 * The debug core accesses this layer only via the operations table returned
 * by @ref debug_port_baremetal_ops, keeping the framework OS-agnostic.
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
#include <stdint.h>
#include "common.h"
#include "debug_port.h"   /*!< Required for debug_port_ops_t */

/****************************** Function declarations ************************************/

/**
 * @brief           Get bare-metal debug port operations table
 *
 * Returns a pointer to the bare-metal debug port operations structure.
 * This structure contains function pointers implementing the minimal
 * services required by the debug framework on non-RTOS systems.
 *
 * @return          Pointer to bare-metal debug port operations table
 *
 * @note
 * Intended for systems running without an RTOS.
 * All operations are lightweight and ISR-safe.
 */
const debug_port_ops_t *debug_port_baremetal_ops(void);

#ifdef __cplusplus
}
#endif

#endif /* DEBUG_USE_BAREMETAL */
#endif /* DEBUG_PORT_BAREMETAL_H */

/****************************** End of file *********************************************/
