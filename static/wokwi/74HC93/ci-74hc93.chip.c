// 74HC93 - contador binario assincrono de 4 bits (divisor por 2 + divisor por 8)
// Borda de descida; MR1 e MR2 juntas em 1 zeram as saidas.
// Chip personalizado para o Wokwi (ELT82F - UTFPR)
#include "wokwi-api.h"
#include <stdlib.h>

typedef struct {
  pin_t cp0, cp1, mr1, mr2, q[4];
  uint32_t div2;   // Q0
  uint32_t div8;   // Q3 Q2 Q1
} chip_state_t;

static uint32_t in_reset(chip_state_t *chip) {
  return pin_read(chip->mr1) && pin_read(chip->mr2);
}

static void update(chip_state_t *chip) {
  if (in_reset(chip)) {
    chip->div2 = 0;
    chip->div8 = 0;
  }
  // Q3..Q1 antes de Q0: Q0 costuma estar ligada a CP1
  for (int i = 0; i < 3; i++) {
    pin_write(chip->q[i + 1], (chip->div8 >> i) & 1);
  }
  pin_write(chip->q[0], chip->div2);
}

static void on_cp0(void *user_data, pin_t pin, uint32_t value) {
  chip_state_t *chip = (chip_state_t *)user_data;
  if (!in_reset(chip)) chip->div2 ^= 1;
  update(chip);
}

static void on_cp1(void *user_data, pin_t pin, uint32_t value) {
  chip_state_t *chip = (chip_state_t *)user_data;
  if (!in_reset(chip)) chip->div8 = (chip->div8 + 1) & 7;
  update(chip);
}

static void on_reset(void *user_data, pin_t pin, uint32_t value) {
  update((chip_state_t *)user_data);
}

void chip_init() {
  chip_state_t *chip = malloc(sizeof(chip_state_t));
  chip->cp0 = pin_init("CP0", INPUT);
  chip->cp1 = pin_init("CP1", INPUT);
  chip->mr1 = pin_init("MR1", INPUT_PULLDOWN);
  chip->mr2 = pin_init("MR2", INPUT_PULLDOWN);
  chip->q[0] = pin_init("Q0", OUTPUT_LOW);
  chip->q[1] = pin_init("Q1", OUTPUT_LOW);
  chip->q[2] = pin_init("Q2", OUTPUT_LOW);
  chip->q[3] = pin_init("Q3", OUTPUT_LOW);
  chip->div2 = 0;
  chip->div8 = 0;

  const pin_watch_config_t cp0_watch = {
    .edge = FALLING, .pin_change = on_cp0, .user_data = chip,
  };
  pin_watch(chip->cp0, &cp0_watch);
  const pin_watch_config_t cp1_watch = {
    .edge = FALLING, .pin_change = on_cp1, .user_data = chip,
  };
  pin_watch(chip->cp1, &cp1_watch);
  const pin_watch_config_t reset_watch = {
    .edge = BOTH, .pin_change = on_reset, .user_data = chip,
  };
  pin_watch(chip->mr1, &reset_watch);
  pin_watch(chip->mr2, &reset_watch);
}
