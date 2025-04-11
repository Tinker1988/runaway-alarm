idf_component_register(
  SRCS 
    "main.c"
    "alarm.c"
  INCLUDE_DIRS 
    "." "../common/"
  REQUIRES
    driver
)
