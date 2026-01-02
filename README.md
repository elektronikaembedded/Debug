# Debug Module for Embedded Systems

A configurable, modular Debug and Logging framework for embedded systems (STM32/ARM Cortex-M).  
Supports multiple transports, RTOS or bare-metal environments, and compile-time log features.

---

## Table of Contents

- [Overview](#overview)  
- [Features](#features)  
- [Folder Structure](#folder-structure)  
- [Getting Started](#getting-started)  
- [Configuration](#configuration)  
- [Initialization](#initialization)  
- [Logging Usage](#logging-usage)  
- [Transport and Port](#transport-and-port)  
- [Build and Size Measurement](#build-and-size-measurement)  
- [Doxygen Documentation](#doxygen-documentation)  
- [License](#license)  

---

## Overview

This Debug Module provides:

- Runtime log level filtering (ERROR, WARN, INFO, DEBUG)  
- Per-module compile-time options for including/excluding debug info  
- Transport-agnostic logging (UART, USB CDC)  
- Thread-safe logging (supports FreeRTOS or bare-metal)  
- Optional timestamp, sequence number, and thread info in logs  

The module is designed for embedded firmware projects and can be easily integrated into STM32CubeIDE or other GCC-based toolchains.

---

## Features

Configurable via `config.h`:

- Enable/disable logs globally  
- Enable/disable sequence numbers, timestamps, thread info  
- Select OS: Bare-metal or FreeRTOS  
- Select transport: UART or USB CDC  
- Thread-safe logging using locks  
- Fixed-size internal buffer for formatting logs  
- Abstract debug transport layer for modularity  
- Ready-to-use drivers for ST and TI UARTs, USB CDC  

---

## Folder Structure

```plaintext
Debug
├── doc/                  # Doxygen or user documentation
├── usage/                # Example usage projects
├── config/
│   └── config.h          # Module configuration
├── core/
│   ├── debug.c
│   └── debug.h
├── port/
│   ├── debug_port.c
│   ├── debug_port.h
│   ├── freertos/
│   │   ├── debug_port_freertos.c
│   │   └── debug_port_freertos.h
│   └── baremetal/
│       ├── debug_port_baremetal.c
│       └── debug_port_baremetal.h
├── transport/
│   ├── debug_transport.c
│   └── debug_transport.h
├── uart/
│   ├── debug_transport_uart_st.c
│   ├── debug_transport_uart_st.h
│   ├── debug_transport_uart_ti.c
│   └── debug_transport_uart_ti.h
└── usb_cdc/
    ├── debug_transport_usb_cdc_st.c
    └── debug_transport_usb_cdc_st.h
---
## Getting Started

### Configuration

Edit `config/config.h` to enable or disable features:

```c
#define DEBUG_ENABLE                YES
#define DEBUG_BUFFER_SIZE           256
#define DEBUG_USE_BAREMETAL         YES
#define DEBUG_USE_FREERTOS          NO
#define DEBUG_USE_USB_CDC           YES
#define DEBUG_USE_UART              NO
#define DEBUG_ENABLE_SEQUENCE_NO    YES
#define DEBUG_ENABLE_TIME_DATE_INFO YES
#define DEBUG_ENABLE_THREAD_INFO    YES
### Initialization

```c
#include "debug.h"
#include "debug_port.h"
#include "debug_transport.h"

int main(void) {
    debug_transport_hal_t transport;
    debug_port_t port;

    // Initialize transport layer (USB CDC, UART, etc.)
    debug_transport_init(&transport);

    // Initialize port layer (Bare-metal or FreeRTOS)
    debug_port_init(&port);

    // Initialize debug module
    int ret = debug_init(&transport, &port);
    if (ret != 0) {
        // Handle initialization error
    }

    // Example logging
    LOG_ERROR("Fatal error: %d\r\n", -1);
    LOG_WARN("Battery low\r\n");
    LOG_INFO("System ready\r\n");
    LOG_DEBUG("Debug info: %d\r\n", 42);

    while (1) {
        // Main loop
    }
}

### Logging Usage
#include "debug.h"

// Basic log
debug_log(LOG_INFO, "System initialized");

// Log with sequence number, timestamp, and thread info
debug_log(LOG_DEBUG, "Sensor value: %d", sensor_val);

// Runtime log level control
debug_set_level(LOG_WARN);
log_level_t lvl = debug_get_level();
