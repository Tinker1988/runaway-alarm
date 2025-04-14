# Create the library
add_library(deep_sleep 
    deep_sleep.c
)

# Link Pico standard library and RTC/Watchdog (if needed)
target_link_libraries(deep_sleep 
    pico_stdlib 
    hardware_rtc 
    hardware_watchdog
)
