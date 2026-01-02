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
