// 74HC74 - dois flip-flops D, borda de subida, preset e clear assincronos ativos em 0
// Chip personalizado para o Wokwi (ELT82F - UTFPR)
#include "wokwi-api.h"
#include <stdlib.h>

typedef struct {
  pin_t clk, d, pre_n, clr_n, q, q_n;
  uint32_t state;
} ff_t;

static void ff_output(ff_t *ff) {
  uint32_t pre = pin_read(ff->pre_n) == LOW;
  uint32_t clr = pin_read(ff->clr_n) == LOW;
  if (pre && clr) {            // combinacao a evitar: as duas saidas em 1
    pin_write(ff->q, HIGH);
    pin_write(ff->q_n, HIGH);
    return;
  }
  if (pre) ff->state = 1;
  if (clr) ff->state = 0;
  pin_write(ff->q, ff->state ? HIGH : LOW);
  pin_write(ff->q_n, ff->state ? LOW : HIGH);
}

static void on_clock(void *user_data, pin_t pin, uint32_t value) {
  ff_t *ff = (ff_t *)user_data;
  ff->state = pin_read(ff->d) ? 1 : 0;
  ff_output(ff);
}

static void on_async(void *user_data, pin_t pin, uint32_t value) {
  ff_output((ff_t *)user_data);
}

static void ff_init(const char *clk, const char *d, const char *pre_n, const char *clr_n,
                    const char *q, const char *q_n) {
  ff_t *ff = malloc(sizeof(ff_t));
  ff->clk = pin_init(clk, INPUT);
  ff->d = pin_init(d, INPUT);
  ff->pre_n = pin_init(pre_n, INPUT_PULLUP);
  ff->clr_n = pin_init(clr_n, INPUT_PULLUP);
  ff->q = pin_init(q, OUTPUT_LOW);
  ff->q_n = pin_init(q_n, OUTPUT_HIGH);
  ff->state = 0;

  const pin_watch_config_t clock_watch = {
    .edge = RISING, .pin_change = on_clock, .user_data = ff,
  };
  pin_watch(ff->clk, &clock_watch);
  const pin_watch_config_t async_watch = {
    .edge = BOTH, .pin_change = on_async, .user_data = ff,
  };
  pin_watch(ff->pre_n, &async_watch);
  pin_watch(ff->clr_n, &async_watch);
}

void chip_init() {
  ff_init("1CLK", "1D", "1PRE_N", "1CLR_N", "1Q", "1Q_N");
  ff_init("2CLK", "2D", "2PRE_N", "2CLR_N", "2Q", "2Q_N");
}
