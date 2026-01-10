/**
 * @file      debug_transport_uart_st.c
 * @brief     UART debug transport implementation (STM32)
 * @version   1.0.0
 * @date      2026-01-02
 * @author    Sarath S
 *
 * @details
 * This module implements the UART-based debug transport layer for STM32
 * microcontrollers.
 *
 * It provides the init, deinit, and write operations required by the
 * debug framework to transmit log data over UART.
 *
 * The UART peripheral is expected to be initialized externally
 * (e.g., via HAL_UART_Init or STM32CubeMX configuration).
 *
 * @par Contact
 * elektronikaembedded@gmail.com
 *
 * @par Website
 * https://elektronikaembedded.wordpress.com
 */

#include "config.h"

#if DEBUG_USE_UART

/*******************************************************************************
 * Includes
 *******************************************************************************/
#include "debug_transport_uart_st.h"
#include "debug_transport.h"
#include "stm32f4xx_hal.h"   /* Replace with MCU-specific HAL header if needed */

/*******************************************************************************
 * Private Function Prototypes (Static)
 *******************************************************************************/
static int uart_init(void);
static int uart_deinit(void);
static int uart_write(const uint8_t *data, size_t len);

/*******************************************************************************
 * Private Variables (Static)
 *******************************************************************************/

/**
 * @brief UART transport operations table
 */
static const debug_transport_ops_t DEBUG_TRANSPORT_UART =
{
    .init   = uart_init,
    .deinit = uart_deinit,
    .write  = uart_write,
};

/*******************************************************************************
 * Private Function Definitions (Static)
 *******************************************************************************/

/**
 * @brief Initialize the UART debug transport.
 *
 * @retval 0  Initialization successful.
 *
 * @note
 * The UART peripheral must be initialized externally
 * (HAL_UART_Init or STM32CubeMX).
 * No initialization is performed in this function.
 */
static int uart_init(void)
{
    return 0;
}/* End of uart_init() */

/**
 * @brief Deinitialize the UART debug transport.
 *
 * @retval 0  Deinitialization successful.
 *
 * @note
 * Placeholder function. No resources are released in the
 * current implementation.
 */
static int uart_deinit(void)
{
    return 0;
}/* End of uart_deinit() */

/**
 * @brief Write debug data over UART.
 *
 * @param[in] data Pointer to data buffer.
 * @param[in] len  Number of bytes to transmit.
 *
 * @retval >=0  Number of bytes written.
 * @retval -1   Transmission failed or invalid parameters.
 *
 * @note
 * Uses HAL_UART_Transmit() in blocking mode.
 * Ensure that the UART handle `huart_debug` is defined and
 * initialized elsewhere in the application.
 */
static int uart_write(const uint8_t *data, size_t len)
{
    if ((NULL == data) || (0U == len))
    {
        return -1;
    }

    extern UART_HandleTypeDef huart_debug;

    if (HAL_OK == HAL_UART_Transmit(&huart_debug,
                                   (uint8_t *)data,
                                   len,
                                   HAL_MAX_DELAY))
    {
        return (int)len;
    }

    return -1;
}/* End of uart_write() */

/*******************************************************************************
 * Public Function Definitions
 *******************************************************************************/

/**
 * @brief Get UART debug transport operations (STM32).
 *
 * @return Pointer to the UART transport operations table.
 *
 * @note
 * Called by the debug transport selector during framework
 * initialization.
 */
const debug_transport_ops_t *debug_transport_uart_st_ops(void)
{
    return &DEBUG_TRANSPORT_UART;
}/* End of debug_transport_uart_st_ops() */

#endif /* DEBUG_USE_UART */

/*******************************************************************************
 * End of file
 *******************************************************************************/
