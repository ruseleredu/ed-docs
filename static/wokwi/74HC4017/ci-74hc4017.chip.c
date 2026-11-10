// 74HC4017 - contador Johnson de decada com 10 saidas decodificadas
// Avanca na subida de CP0 (com CP1_N em 0) ou na descida de CP1_N (com CP0 em 1).
// MR em 1 leva a contagem a zero. CO_N fica em 1 de 0 a 4 e em 0 de 5 a 9.
// Chip personalizado para o Wokwi (ELT82F - UTFPR)
#include "wokwi-api.h"
#include <stdlib.h>

typedef struct {
  pin_t cp0, cp1_n, mr, q[10], co_n;
  uint32_t count;
} chip_state_t;

static void update(chip_state_t *chip) {
  if (pin_read(chip->mr)) chip->count = 0;
  for (uint32_t i = 0; i < 10; i++) {
    if (i != chip->count) pin_write(chip->q[i], LOW);
  }
  pin_write(chip->q[chip->count], HIGH);
  pin_write(chip->co_n, chip->count < 5 ? HIGH : LOW);
}

static void advance(chip_state_t *chip) {
  if (!pin_read(chip->mr)) chip->count = (chip->count + 1) % 10;
  update(chip);
}

static void on_cp0(void *user_data, pin_t pin, uint32_t value) {
  chip_state_t *chip = (chip_state_t *)user_data;
  if (pin_read(chip->cp1_n) == LOW) advance(chip);
}

static void on_cp1(void *user_data, pin_t pin, uint32_t value) {
  chip_state_t *chip = (chip_state_t *)user_data;
  if (pin_read(chip->cp0) == HIGH) advance(chip);
}

static void on_reset(void *user_data, pin_t pin, uint32_t value) {
  update((chip_state_t *)user_data);
}

void chip_init() {
  static const char *Q[10] = {"Q0", "Q1", "Q2", "Q3", "Q4", "Q5", "Q6", "Q7", "Q8", "Q9"};
  chip_state_t *chip = malloc(sizeof(chip_state_t));
  chip->cp0 = pin_init("CP0", INPUT);
  chip->cp1_n = pin_init("CP1_N", INPUT_PULLDOWN);
  chip->mr = pin_init("MR", INPUT_PULLDOWN);
  chip->q[0] = pin_init(Q[0], OUTPUT_HIGH);
  for (int i = 1; i < 10; i++) chip->q[i] = pin_init(Q[i], OUTPUT_LOW);
  chip->co_n = pin_init("CO_N", OUTPUT_HIGH);
  chip->count = 0;

  const pin_watch_config_t cp0_watch = {
    .edge = RISING, .pin_change = on_cp0, .user_data = chip,
  };
  pin_watch(chip->cp0, &cp0_watch);
  const pin_watch_config_t cp1_watch = {
    .edge = FALLING, .pin_change = on_cp1, .user_data = chip,
  };
  pin_watch(chip->cp1_n, &cp1_watch);
  const pin_watch_config_t reset_watch = {
    .edge = BOTH, .pin_change = on_reset, .user_data = chip,
  };
  pin_watch(chip->mr, &reset_watch);
}
