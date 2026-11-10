// CD4511 - decodificador BCD para 7 segmentos (catodo comum) com latch
// LT_N = 0 acende tudo; BL_N = 0 apaga tudo; LE = 1 retem o ultimo digito.
// Codigos de 10 a 15 apagam o display.
// Chip personalizado para o Wokwi (ELT82F - UTFPR)
#include "wokwi-api.h"
#include <stdlib.h>

typedef struct {
  pin_t in[4];      // A B C D
  pin_t lt_n, bl_n, le;
  pin_t seg[7];     // a b c d e f g
  uint32_t latch;
} chip_state_t;

// bit 0 = a ... bit 6 = g
static const uint8_t DIGITS[10] = {
  0x3F, // 0: a b c d e f
  0x06, // 1: b c
  0x5B, // 2: a b d e g
  0x4F, // 3: a b c d g
  0x66, // 4: b c f g
  0x6D, // 5: a c d f g
  0x7C, // 6: c d e f g
  0x07, // 7: a b c
  0x7F, // 8: todos
  0x67, // 9: a b c f g
};

static void update(chip_state_t *chip) {
  if (pin_read(chip->le) == LOW) {
    chip->latch = 0;
    for (int i = 0; i < 4; i++) {
      chip->latch |= (pin_read(chip->in[i]) ? 1u : 0u) << i;
    }
  }
  uint8_t pattern = chip->latch < 10 ? DIGITS[chip->latch] : 0;
  if (pin_read(chip->bl_n) == LOW) pattern = 0;
  if (pin_read(chip->lt_n) == LOW) pattern = 0x7F;
  for (int i = 0; i < 7; i++) {
    pin_write(chip->seg[i], (pattern >> i) & 1);
  }
}

static void on_change(void *user_data, pin_t pin, uint32_t value) {
  update((chip_state_t *)user_data);
}

void chip_init() {
  static const char *IN[4] = {"A", "B", "C", "D"};
  static const char *SEG[7] = {"SEG_A", "SEG_B", "SEG_C", "SEG_D", "SEG_E", "SEG_F", "SEG_G"};
  chip_state_t *chip = malloc(sizeof(chip_state_t));
  const pin_watch_config_t watch = {
    .edge = BOTH, .pin_change = on_change, .user_data = chip,
  };
  for (int i = 0; i < 4; i++) {
    chip->in[i] = pin_init(IN[i], INPUT);
    pin_watch(chip->in[i], &watch);
  }
  chip->lt_n = pin_init("LT_N", INPUT_PULLUP);
  chip->bl_n = pin_init("BL_N", INPUT_PULLUP);
  chip->le = pin_init("LE", INPUT_PULLDOWN);
  pin_watch(chip->lt_n, &watch);
  pin_watch(chip->bl_n, &watch);
  pin_watch(chip->le, &watch);
  for (int i = 0; i < 7; i++) chip->seg[i] = pin_init(SEG[i], OUTPUT_LOW);
  chip->latch = 0;
  update(chip);
}
