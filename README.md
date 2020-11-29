## MAX7219

Helper library for programming a MAX7219 via SPI with STM32 HAL.

### Example

```C
/* Project setup example */
#define SPI1_CS_Pin GPIO_PIN_4
#define SPI1_CS_GPIO_Port GPIOA
SPI_HandleTypeDef hspi1;

/* Configuration */
max7219_config_t max7219_config;
max7219_config.hspi = &hspi1;
max7219_config.port_cs = SPI1_CS_GPIO_Port;
max7219_config.pin_cs = SPI1_CS_Pin;
max7219_config.intensity = 5;
max7219_config.digits = 3;
max7219_init(&max7219_config);

/* Use */
max7219_show_decimal(&max7219_config, 42);
```

### About the MAX7219

The MAX7219 is a common-cathode display driver that can drive up to 8 7-segment
LEDs or 64 individual LEDs. See the
[datasheet](https://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf)
for details.

Note that peak current is set with an external resistor on pin 18. The
datasheet has a section on how to select the correct resistor value. Note that
you need to do this whether or not you use the `intensity` configuration,
because the MAX7219 will power up into a test mode with all LEDs blazing
*before* your configurations takes effect. You could probably burn out your
nice display by accident this way.

