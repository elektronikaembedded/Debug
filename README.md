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
- [How to Use](#how-to-use)  
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

debug
├── inc/                  
├── src/                
├── template/
    └── debug_config.h.example          # Config file template
    └── debug_transport.c.uart.example  # debug_transport.c uart file template
    └── debug_transport.c.usb.example   # debug_transport.c usb file template

```
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

```
### Initialization

```c
#include "debug.h"
#include "osal.h" // External dependency, include in your project

static debug_transport_t debug_trans; // Transport instance
static osal_t osal; // osal instance
int main(void) 
{
     MX_USB_DEVICE_Init(); // USB as transport

    // Initialize the osal layer
    osal_init(&osal, get_osal_ops());

    // Initialize transport layer (USB CDC, UART, etc.)
    debug_transport_init(&debug_trans);

    // Initialize debug module
    int ret = debug_port_init(&port);
    if (ret != 0) 
    {
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

```
### Logging Usage
```c
#include "debug.h"

// Basic log
debug_log(LOG_INFO, "System initialized");

// Log with sequence number, timestamp, and thread info
debug_log(LOG_DEBUG, "Sensor value: %d", sensor_val);

// Runtime log level control
debug_set_level(LOG_WARN);
log_level_t lvl = debug_get_level();

```
### How to use


### License

This project is licensed under the MIT License. See LICENSE
 for details.
