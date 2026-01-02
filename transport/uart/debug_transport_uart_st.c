/****************************************************************************************
 * @file        debug_transport_uart_st.c
 * @author      Sarath S
 * @date        2026-01-02
 * @version     1.0
 * @brief       STM32 UART debug transport implementation
 *
 * @details
 * This file implements the UART-based debug transport layer for STM32 MCUs.
 * It provides standard init, deinit, and write operations required by
 * the debug framework to send log data over UART.
 *
 * The UART peripheral is assumed to be initialized externally (e.g., via
 * HAL_UART_Init or STM32CubeMX configuration).
 *
 * @contact     elektronikaembedded@gmail.com
 * @website     https://elektronikaembedded.wordpress.com
 ****************************************************************************************/

#include "config.h"

#if DEBUG_USE_UART

/****************************** Header include files ************************************/
#include "debug_transport_uart_st.h"
#include "debug_transport.h"
#include "stm32f4xx_hal.h"   /* Replace with MCU-specific HAL header if needed */

/****************************** Static function prototypes ******************************/
static int uart_init(void);
static int uart_deinit(void);
static int uart_write(const uint8_t *data, size_t len);

/****************************** Static variable definitions ******************************/
static const debug_transport_ops_t DEBUG_TRANSPORT_UART =
{
    .init   = uart_init,
    .deinit = uart_deinit,
    .write  = uart_write,
};

/****************************** Function definitions ************************************/

/*!----------------------------------------------------------------------------
 * \brief           Initialize UART debug transport
 * \return          0 on success
 * \note
 * UART peripheral must be initialized externally (HAL_UART_Init or CubeMX).
 * No initialization is performed here.
 *---------------------------------------------------------------------------*/
static int uart_init(void)
{
    return 0;
} /* End of uart_init() */

/*!----------------------------------------------------------------------------
 * \brief           Deinitialize UART debug transport
 * \return          0 on success
 * \note
 * Placeholder function. No resources are freed in the current implementation.
 *---------------------------------------------------------------------------*/
static int uart_deinit(void)
{
    return 0;
} /* End of uart_deinit() */

/*!----------------------------------------------------------------------------
 * \brief           Write debug data over UART
 * \param[in]       data   Pointer to data buffer
 * \param[in]       len    Number of bytes to transmit
 * \return          Number of bytes written, or -1 on error
 * \note
 * Uses HAL_UART_Transmit in blocking mode. Ensure that `huart_debug`
 * is defined and initialized elsewhere in the project.
 *---------------------------------------------------------------------------*/
static int uart_write(const uint8_t *data, size_t len)
{
    if ((NULL == data) || (0U == len))
    {
        return -1;
    }

    extern UART_HandleTypeDef huart_debug;

    if (HAL_OK == HAL_UART_Transmit(&huart_debug, (uint8_t *)data, len, HAL_MAX_DELAY))
    {
        return (int)len;
    }

    return -1;
} /* End of uart_write() */

/*!----------------------------------------------------------------------------
 * \brief           Get UART transport operations table
 * \return          Pointer to UART transport operations structure
 * \note
 * Called by the debug core during initialization to obtain transport functions.
 *---------------------------------------------------------------------------*/
const debug_transport_ops_t *debug_transport_uart_st_ops(void)
{
    return &DEBUG_TRANSPORT_UART;
} /* End of debug_transport_uart_st_ops() */

#endif /* DEBUG_USE_UART */

/****************************** End of file *********************************************/
