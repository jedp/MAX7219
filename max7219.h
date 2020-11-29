/**
 * Library to program the MAX7219 via SPI using STM32 HAL.
 *
 * By Jed Parsons.
 */

#ifndef __MAX7219_H
#define __MAX7219_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "stm32f3xx_hal.h" // Change to whatever you're using.

/**
 * MAX7219 Datasheet, Table 2, "Register Address Map".
 */
enum {
  R_NO_OP = 0x0,
  R_DIGIT_0,
  R_DIGIT_1,
  R_DIGIT_2,
  R_DIGIT_3,
  R_DIGIT_4,
  R_DIGIT_5,
  R_DIGIT_6,
  R_DIGIT_7,
  R_DECODE_MODE,
  R_INTENSITY,
  R_SCAN_LIMIT,
  R_SHUTDOWN,
  R_DISPLAY_TEST = 0xF
};

/**
 * MAX7219 Datasheet, Table 5, "Code B Font".
 */
enum {
  B_CODE_0 = 0x0,
  B_CODE_1,
  B_CODE_2,
  B_CODE_3,
  B_CODE_4,
  B_CODE_5,
  B_CODE_6,
  B_CODE_7,
  B_CODE_8,
  B_CODE_9,
  B_CODE_HYPHEN,
  B_CODE_E,
  B_CODE_H,
  B_CODE_L,
  B_CODE_P,
  B_CODE_BLANK
};

typedef struct {
  SPI_HandleTypeDef *hspi;
  GPIO_TypeDef * port_cs;
  uint16_t pin_cs;
  uint8_t intensity; // Valid values: 0x00 to 0x0F
  uint8_t digits;    // Valid values: 1 through 8
} max7219_config_t;

/**
 * Initialize the 7219.
 *
 * - Blank all digits
 * - Set operation to normal
 * - Configure display digits and brightness according to config
 */
void max7219_init(max7219_config_t *config);

/**
 * Configure the MAX7219 according to the values in config.
 */
void max7219_configure(max7219_config_t *config);

/**
 * Display a test sequence.
 */
void max7219_say_hi(max7219_config_t *config);

/**
 * Display a decimal value.
 *
 * Does not necessarily configure the device according to config.
 * Use max7219_configure if you want to change intensity etc.
 */
void max7219_show_decimal(max7219_config_t *config, int value);

/**
 * Set a register in the MAX7219.
 *
 * Does not necessarily configure the device according to config.
 * Use max7219_configure if you want to change intensity etc.
 */
void max7219_set_register(max7219_config_t *config, uint8_t reg, uint8_t value);

/**
 * Helper function to perform integer exponentiation quickly.
 */
int ipow10(int exp);

#endif  // __MAX7219_H

