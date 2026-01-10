/**
 * @file      config.h
 * @brief     Debug and logging framework configuration.
 * @version   1.1
 * @date      2025-08-07
 * @author    Sarath S
 *
 * @details
 * This header defines all compile-time configuration options for the
 * debug and logging framework.
 *
 * It allows the application to:
 *  - Enable or completely disable the debug framework
 *  - Select the execution environment (bare-metal or RTOS)
 *  - Choose the debug output transport (UART, USB CDC, etc.)
 *  - Control log formatting features such as timestamps, sequence numbers,
 *    and thread or task information
 *
 * All options in this file are evaluated at compile time.
 * When a feature is disabled, the related code is entirely removed by the
 * compiler and linker, ensuring zero runtime and memory overhead.
 *
 * The debug framework is portable across multiple vendors
 * (STM32, NXP, TI) using abstracted port and transport layers.
 *
 * @note
 * Modify this file with care. Some options are mutually exclusive and must
 * be configured consistently to avoid build or runtime issues.
 *
 * @par Contact
 * elektronikaembedded@gmail.com
 *
 * @par Website
 * https://elektronikaembedded.wordpress.com
 */

/** @defgroup DEBUG_CONFIG Debug Configuration
 *  @brief Compile-time configuration for debug framework.
 *  @{
 */

#ifndef CONFIG_H
#define CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Common Macros
 *******************************************************************************/

/**
 * @def YES
 * @brief Boolean macro indicating enabled state.
 */
#define YES   1

/**
 * @def NO
 * @brief Boolean macro indicating disabled state.
 */
#define NO    0

/*******************************************************************************
 * Debug Framework Master Control
 *******************************************************************************/

/**
 * @def DEBUG_ENABLE
 * @brief Global enable switch for the debug framework.
 *
 * @note
 * - Set to @ref YES to enable debug logging.
 * - Set to @ref NO to exclude all debug-related code.
 */
#define DEBUG_ENABLE           YES

/**
 * @def DEBUG_BUFFER_SIZE
 * @brief Size of the internal debug message buffer in bytes.
 */
#define DEBUG_BUFFER_SIZE      256

/*******************************************************************************
 * Platform / OS Selection
 *******************************************************************************/

/**
 * @def DEBUG_USE_BAREMETAL
 * @brief Enable debug support for bare-metal applications.
 */
#define DEBUG_USE_BAREMETAL    YES

/**
 * @def DEBUG_USE_FREERTOS
 * @brief Enable debug support for FreeRTOS-based applications.
 *
 * @note Only one platform option should be enabled at any given time.
 */
#define DEBUG_USE_FREERTOS     NO

/* Compile-time guard for mutual exclusivity */
#if (DEBUG_USE_BAREMETAL == YES && DEBUG_USE_FREERTOS == YES)
#error "Only one execution environment can be selected (Baremetal OR FreeRTOS)."
#endif

/*******************************************************************************
 * Debug Transport Selection
 *******************************************************************************/

/**
 * @def DEBUG_USE_USB_CDC
 * @brief Enable USB CDC as the debug output transport.
 */
#define DEBUG_USE_USB_CDC      YES

/**
 * @def DEBUG_USE_UART
 * @brief Enable UART as the debug output transport.
 */
#define DEBUG_USE_UART         NO

/* Compile-time guard for transport exclusivity */
#if (DEBUG_USE_USB_CDC == YES && DEBUG_USE_UART == YES)
#error "Select only one debug transport (USB CDC OR UART)."
#endif

/*******************************************************************************
 * Log Formatting Options
 *******************************************************************************/

/**
 * @def DEBUG_ENABLE_SEQUENCE_NO
 * @brief Enable sequence numbers in log output.
 */
#define DEBUG_ENABLE_SEQUENCE_NO      YES

/**
 * @def DEBUG_ENABLE_TIME_DATE_INFO
 * @brief Enable timestamp information in log output.
 */
#define DEBUG_ENABLE_TIME_DATE_INFO   YES

/**
 * @def DEBUG_ENABLE_THREAD_INFO
 * @brief Enable thread or task information in log output.
 */
#define DEBUG_ENABLE_THREAD_INFO      YES

/**
 * @def DEBUG_ENABLE_MODULE_LOG
 * @brief Enable module-based log filtering.
 */
#define DEBUG_ENABLE_MODULE_LOG       NO

/*******************************************************************************
 * Vendor Selection
 *******************************************************************************/

/**
 * @def DEBUG_VENDOR_STM32
 * @brief Select STM32 as the target vendor platform.
 */
#define DEBUG_VENDOR_STM32    1

/* Uncomment exactly one vendor option below */
//#define DEBUG_VENDOR_NXP    1
//#define DEBUG_VENDOR_TI     1

/* Compile-time guard for vendor selection */
#if (DEBUG_VENDOR_STM32 + DEBUG_VENDOR_NXP + DEBUG_VENDOR_TI) != 1
#error "Exactly one vendor must be selected."
#endif

#ifdef __cplusplus
}
#endif

#endif /* CONFIG_H */

/** @} */ // End of DEBUG_CONFIG

/*******************************************************************************
 * End of file
 *******************************************************************************/
