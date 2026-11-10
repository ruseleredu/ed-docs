// 74HC283 - somador completo de 4 bits
// Chip personalizado para o Wokwi (ELT82F - UTFPR)
#include "wokwi-api.h"
#include <stdlib.h>

typedef struct {
  pin_t a[4], b[4], c0, s[4], c4;
} chip_state_t;

static void update(chip_state_t *chip) {
  uint32_t a = 0, b = 0;
  for (int i = 0; i < 4; i++) {
    a |= (pin_read(chip->a[i]) ? 1u : 0u) << i;
    b |= (pin_read(chip->b[i]) ? 1u : 0u) << i;
  }
  uint32_t sum = a + b + (pin_read(chip->c0) ? 1u : 0u);
  for (int i = 0; i < 4; i++) {
    pin_write(chip->s[i], (sum >> i) & 1);
  }
  pin_write(chip->c4, (sum >> 4) & 1);
}

static void on_change(void *user_data, pin_t pin, uint32_t value) {
  update((chip_state_t *)user_data);
}

void chip_init() {
  static const char *A[4] = {"A1", "A2", "A3", "A4"};
  static const char *B[4] = {"B1", "B2", "B3", "B4"};
  static const char *S[4] = {"S1", "S2", "S3", "S4"};
  chip_state_t *chip = malloc(sizeof(chip_state_t));
  const pin_watch_config_t watch = {
    .edge = BOTH, .pin_change = on_change, .user_data = chip,
  };
  for (int i = 0; i < 4; i++) {
    chip->a[i] = pin_init(A[i], INPUT);
    chip->b[i] = pin_init(B[i], INPUT);
    chip->s[i] = pin_init(S[i], OUTPUT_LOW);
    pin_watch(chip->a[i], &watch);
    pin_watch(chip->b[i], &watch);
  }
  chip->c0 = pin_init("C0", INPUT);
  chip->c4 = pin_init("C4", OUTPUT_LOW);
  pin_watch(chip->c0, &watch);
  update(chip);
}
