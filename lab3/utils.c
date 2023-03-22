#include <lcom/lcf.h>

#include <stdint.h>
#include "utils.h"

#ifdef LAB3
extern int cnt;
#endif

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  uint16_t mask = BIT(0) | BIT(1) | BIT(2) | BIT(3) | BIT(4) | BIT(5) | BIT (6) | BIT(7);
  *lsb = val & mask;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  uint16_t mask = BIT(0) | BIT(1) | BIT(2) | BIT(3) | BIT(4) | BIT(5) | BIT (6) | BIT(7);
  val >>= 8;
  *msb = val & mask;
  return 0;
}


int (util_sys_inb)(int port, uint8_t *value) {
  #ifdef LAB3
  cnt++;
  #endif
  uint32_t value1 = 0;
  if(sys_inb(port, &value1) != 0) return 1;
  *value = (uint8_t)value1;
  return 0;
}

