// 74HC08 - quatro portas AND de 2 entradas
// Chip personalizado para o Wokwi (ELT82F - UTFPR)
#include "wokwi-api.h"
#include <stdlib.h>

#define GATES 4
#define INPUTS 2

static const char *IN_NAMES[GATES][INPUTS] = {
  {"1A", "1B"},
  {"2A", "2B"},
  {"3A", "3B"},
  {"4A", "4B"}
};
static const char *OUT_NAMES[GATES] = {"1Y", "2Y", "3Y", "4Y"};

typedef struct {
  pin_t in[GATES][INPUTS];
  pin_t out[GATES];
} chip_state_t;

// Funcao logica de uma porta: in[] contem os niveis das entradas (0 ou 1)
static uint32_t gate(const uint32_t *in) {
  return in[0] && in[1];
}

static void update(chip_state_t *chip) {
  for (int g = 0; g < GATES; g++) {
    uint32_t in[INPUTS];
    for (int i = 0; i < INPUTS; i++) in[i] = pin_read(chip->in[g][i]) ? 1 : 0;
    pin_write(chip->out[g], gate(in) ? HIGH : LOW);
  }
}

static void on_change(void *user_data, pin_t pin, uint32_t value) {
  update((chip_state_t *)user_data);
}

void chip_init() {
  chip_state_t *chip = malloc(sizeof(chip_state_t));
  const pin_watch_config_t watch = {
    .edge = BOTH, .pin_change = on_change, .user_data = chip,
  };
  for (int g = 0; g < GATES; g++) {
    chip->out[g] = pin_init(OUT_NAMES[g], OUTPUT);
    for (int i = 0; i < INPUTS; i++) {
      chip->in[g][i] = pin_init(IN_NAMES[g][i], INPUT);
      pin_watch(chip->in[g][i], &watch);
    }
  }
  update(chip);
}
