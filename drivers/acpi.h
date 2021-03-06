/* 
    MIT License
    (c) Antonio de Haro 2020

    See attached LICENSE file for more info

    drivers/acpi.h:
    Header file for the ACPI driver

    This has been mostly taken from a marvelous code example. 
    See: https://forum.osdev.org/viewtopic.php?t=16990

*/

#ifndef DRIVERS_ACPI
#define DRIVERS_ACPI

#include <cpu/ports.h>
#include <cpu/timer.h>
#include <drivers/screen.h>
#include <libc/mem.h>
#include <libc/type.h>

/* Initialises the ACPI */
int32_t acpi_init();

/* Instructs the power controller to invoke a system shutdown */
void acpi_poweroff();

#endif
