/*!----------------------------------------------------------------------------
 * @file        config.h
 * @author      Sarath S
 * @date        2025-08-07
 * @version     1.0
 *
 * @brief       Debug and logging framework configuration
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
 * The debug framework is designed to be portable across multiple vendors
 * (STM32, NXP, TI) by using abstracted port and transport layers.
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
 *---------------------------------------------------------------------------*/

#ifndef CONFIG_H
#define CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*/
/* Common macros                                                              */
/*============================================================================*/

/**
 * @def YES
 * @brief Boolean macro indicating an enabled state.
 */
#define YES                             1

/**
 * @def NO
 * @brief Boolean macro indicating a disabled state.
 */
#define NO                              0

/*============================================================================*/
/* Debug framework master control                                             */
/*============================================================================*/

/**
 * @def DEBUG_ENABLE
 * @brief Global enable switch for the debug framework.
 *
 * When set to @ref YES, debug logging and related features are enabled.
 * When set to @ref NO, all debug-related code is excluded from the build.
 */
#define DEBUG_ENABLE                    YES

/**
 * @def DEBUG_BUFFER_SIZE
 * @brief Size of the internal debug message buffer (in bytes).
 *
 * This buffer is used to format log messages before they are sent
 * through the selected debug transport.
 */
#define DEBUG_BUFFER_SIZE               256

/*============================================================================*/
/* Platform / OS selection                                                    */
/*============================================================================*/

/**
 * @def DEBUG_USE_BAREMETAL
 * @brief Enable debug support for bare-metal applications.
 *
 * Set to @ref YES when the application runs without an RTOS.
 */
#define DEBUG_USE_BAREMETAL             YES

/**
 * @def DEBUG_USE_FREERTOS
 * @brief Enable debug support for FreeRTOS-based applications.
 *
 * Set to @ref YES when the application runs on FreeRTOS.
 *
 * @note
 * Only one platform option should be enabled at any given time.
 */
#define DEBUG_USE_FREERTOS              NO

/*============================================================================*/
/* Debug transport selection                                                  */
/*============================================================================*/

/**
 * @def DEBUG_USE_USB_CDC
 * @brief Enable USB CDC as the debug output transport.
 */
#define DEBUG_USE_USB_CDC               YES

/**
 * @def DEBUG_USE_UART
 * @brief Enable UART as the debug output transport.
 */
#define DEBUG_USE_UART                  NO

/**
 * @note
 * All DEBUG_USE_* options are compile-time selections.
 * Any disabled transport is completely removed from the final binary.
 */

/*============================================================================*/
/* Log formatting options                                                     */
/*============================================================================*/

/**
 * @def DEBUG_ENABLE_SEQUENCE_NO
 * @brief Enable sequence numbers in log output.
 *
 * When enabled, each log message is prefixed with an incremental counter,
 * making it easier to trace execution flow.
 */
#define DEBUG_ENABLE_SEQUENCE_NO        YES

/**
 * @def DEBUG_ENABLE_TIME_DATE_INFO
 * @brief Enable timestamp information in log output.
 *
 * The actual time source depends on the selected platform and
 * the underlying port implementation.
 */
#define DEBUG_ENABLE_TIME_DATE_INFO     YES

/**
 * @def DEBUG_ENABLE_THREAD_INFO
 * @brief Enable thread or task information in log output.
 *
 * On bare-metal systems, this typically resolves to identifiers such as
 * "MAIN" or "ISR".
 * On RTOS-based systems, this may resolve to the task name.
 */
#define DEBUG_ENABLE_THREAD_INFO        YES

/**
 * @def DEBUG_ENABLE_MODULE_LOG
 * @brief Enable module-based log filtering.
 *
 * When enabled, log messages can be selectively enabled or disabled
 * based on module identifiers.
 */
#define DEBUG_ENABLE_MODULE_LOG         NO

/*============================================================================*/
/* Vendor selection                                                          */
/*============================================================================*/

/**
 * @def DEBUG_VENDOR_STM32
 * @brief Select STM32 as the target vendor platform.
 */
#define DEBUG_VENDOR_STM32              1

/* Uncomment exactly one vendor option below */
//#define DEBUG_VENDOR_NXP              1
//#define DEBUG_VENDOR_TI               1

#ifdef __cplusplus
}
#endif

#endif /* CONFIG_H */

/****************************** End of file ***********************************/
