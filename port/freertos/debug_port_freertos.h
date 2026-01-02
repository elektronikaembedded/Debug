/****************************************************************************************
 * @file        debug_port_baremetal.h
 * @author      Sarath S
 * @date        2026-01-02
 * @version     1.0
 * @brief       Bare-metal debug port interface
 *
 * @details
 * Declares the bare-metal debug port layer for the debug framework.
 * Provides minimal OS abstraction services such as:
 *   - Locking / unlocking (no-op)
 *   - ISR detection (stub or processor-specific)
 *   - Timestamp retrieval (stub)
 *   - Thread name access (always "MAIN" or "ISR")
 * Exposes only the getter for the operations table.
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

/****************************** Function prototypes ************************************/

/*!----------------------------------------------------------------------------
 * \brief           Get bare-metal debug port operations table
 * \param[in]       None
 * \param[out]      None
 * \param[in/out]   None
 * \return          Pointer to bare-metal debug port operations table
 * \note            Intended for systems without an RTOS
 *---------------------------------------------------------------------------*/
const debug_port_ops_t *debug_port_baremetal_ops(void);

#ifdef __cplusplus
}
#endif

#endif /* DEBUG_USE_BAREMETAL */
#endif /* DEBUG_PORT_BAREMETAL_H */

/****************************** End of file *********************************************/
