/**
 * max7219.c
 *
 * By Jed Parsons.
 */

#include "max7219.h"

#define TIMEOUT_MS 100

void max7219_init(max7219_config_t *config) {
  max7219_set_register(config, R_DIGIT_0, B_CODE_BLANK);
  max7219_set_register(config, R_DIGIT_1, B_CODE_BLANK);
  max7219_set_register(config, R_DIGIT_2, B_CODE_BLANK);
  max7219_set_register(config, R_DIGIT_3, B_CODE_BLANK);
  max7219_set_register(config, R_DIGIT_4, B_CODE_BLANK);
  max7219_set_register(config, R_DIGIT_5, B_CODE_BLANK);
  max7219_set_register(config, R_DIGIT_6, B_CODE_BLANK);
  max7219_set_register(config, R_DIGIT_7, B_CODE_BLANK);

  /* Set intensity and digits. */
  max7219_configure(config);

  /* Enable B Code mode. */
  max7219_set_register(config, R_DECODE_MODE, 0xFF);

  /* Normal operation: Set both Shutdown = 1 and Test = 0. */
  max7219_set_register(config, R_SHUTDOWN, 0x01);
  max7219_set_register(config, R_DISPLAY_TEST, 0x00);
}

void max7219_configure(max7219_config_t *config) {
  max7219_set_register(config, R_INTENSITY, config->intensity);
  max7219_set_register(config, R_SCAN_LIMIT, config->digits - 1);
}

void max7219_say_hi(max7219_config_t *config) {
  max7219_set_register(config, R_DIGIT_0, B_CODE_BLANK);
  max7219_set_register(config, R_DIGIT_1, B_CODE_H);
  max7219_set_register(config, R_DIGIT_2, B_CODE_1);
}

void max7219_show_decimal(max7219_config_t *config, int value) {
  if (config->digits < 1) {
    return;
  }

  if (value >= ipow10(config->digits)) {
    max7219_set_register(config, R_DIGIT_0, B_CODE_H);
    max7219_set_register(config, R_DIGIT_1, B_CODE_L);
    max7219_set_register(config, R_DIGIT_2, B_CODE_P);
    return;
  }

  int startValue = value;
  int digitOffset = config->digits - 1;

  for (int exp = digitOffset; exp >= 0; --exp) {
    int place = ipow10(exp);

    if (value >= place) {
      max7219_set_register(config, R_DIGIT_0 + digitOffset - exp, B_CODE_0 + (value / place));
      value %= place;
    } else {
      // Either a leading blank, or a 0 in the middle somewhere.
      max7219_set_register(config, R_DIGIT_0 + digitOffset - exp, startValue > place ? B_CODE_0 : B_CODE_BLANK);
    }
  }
}

void max7219_set_register(max7219_config_t *config, uint8_t reg, uint8_t value) {
  uint8_t message[2];
  message[0] = reg;
  message[1] = value;

  HAL_GPIO_WritePin(config->port_cs, config->pin_cs, GPIO_PIN_RESET);
  HAL_SPI_Transmit(config->hspi, (uint8_t *) message, 2, TIMEOUT_MS);
  HAL_GPIO_WritePin(config->port_cs, config->pin_cs, GPIO_PIN_SET);
}

/**
 * Exponentiation by squaring.
 */
int ipow10(int exp) {
  if (exp < 1) return 1;

  int base = 10;
  int result = 1;

  while (1) {
    if (exp & 1) result *= base;

    exp >>= 1;

    if (!exp) break;

    base *= base;
  }

  return result;
}

