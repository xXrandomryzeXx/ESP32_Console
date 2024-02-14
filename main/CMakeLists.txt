set(srcs "display.c"
    "main.c"
    "lcd.c"
    "decode_image.c"
    "button.c"
    "memory.c"
    "spi.c"
    "text.c"
    )

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS "."
                    EMBED_FILES background.jpg)
