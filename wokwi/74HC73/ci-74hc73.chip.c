// 74HC73 - dois flip-flops JK, borda de descida, reset assincrono ativo em 0
// Chip personalizado para o Wokwi (ELT82F - UTFPR)
#include "wokwi-api.h"
#include <stdlib.h>

typedef struct {
  pin_t cp, j, k, r_n, q, q_n;
  uint32_t state;
} ff_t;

static void ff_output(ff_t *ff) {
  pin_write(ff->q, ff->state ? HIGH : LOW);
  pin_write(ff->q_n, ff->state ? LOW : HIGH);
}

static void on_clock(void *user_data, pin_t pin, uint32_t value) {
  ff_t *ff = (ff_t *)user_data;
  if (pin_read(ff->r_n) == LOW) {
    ff->state = 0;
  } else {
    uint32_t j = pin_read(ff->j), k = pin_read(ff->k);
    if (j && k) ff->state = !ff->state;
    else if (j) ff->state = 1;
    else if (k) ff->state = 0;
  }
  ff_output(ff);
}

static void on_reset(void *user_data, pin_t pin, uint32_t value) {
  ff_t *ff = (ff_t *)user_data;
  if (value == LOW) {
    ff->state = 0;
    ff_output(ff);
  }
}

static void ff_init(const char *cp, const char *j, const char *k, const char *r_n,
                    const char *q, const char *q_n) {
  ff_t *ff = malloc(sizeof(ff_t));
  ff->cp = pin_init(cp, INPUT);
  ff->j = pin_init(j, INPUT);
  ff->k = pin_init(k, INPUT);
  ff->r_n = pin_init(r_n, INPUT_PULLUP);
  ff->q = pin_init(q, OUTPUT_LOW);
  ff->q_n = pin_init(q_n, OUTPUT_HIGH);
  ff->state = 0;

  const pin_watch_config_t clock_watch = {
    .edge = FALLING, .pin_change = on_clock, .user_data = ff,
  };
  pin_watch(ff->cp, &clock_watch);
  const pin_watch_config_t reset_watch = {
    .edge = BOTH, .pin_change = on_reset, .user_data = ff,
  };
  pin_watch(ff->r_n, &reset_watch);
}

void chip_init() {
  ff_init("1CP", "1J", "1K", "1R_N", "1Q", "1Q_N");
  ff_init("2CP", "2J", "2K", "2R_N", "2Q", "2Q_N");
}
