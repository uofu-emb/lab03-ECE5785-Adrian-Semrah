# This is the main binary. List your C files here.
add_executable(threads
    threads.c
    threads_functions.c
    )

pico_set_program_name(threads "threads")
pico_set_program_version(threads "0.1")

# Modify the below lines to enable/disable output over UART/USB.
# These are mutually exclusive, only set one.
pico_enable_stdio_uart(threads 0)
pico_enable_stdio_usb(threads 1)

# Add the libraries to the build
target_link_libraries(threads PRIVATE
  pico_stdlib
  pico_multicore
  pico_async_context_freertos
  FreeRTOS-Kernel-Heap4
)

# The CYW43 is the Wifi/Bluetooth module. If the board is set to pico_w, this
# variable will be set and the wireless libraries added.
if(PICO_CYW43_SUPPORTED)
    # For led support on pico_w
    target_link_libraries(threads PRIVATE
        pico_cyw43_arch_none
    )
endif()

# Another pico macro which generates the uf2 file in addition to the elf
pico_add_extra_outputs(threads)