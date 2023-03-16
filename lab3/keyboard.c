#include <lcom/lcf.h>

#include "keyboard.h"
#include "i8042.h"

int kbc_hook_id = 0;
uint8_t data = 0;

int (kbc_subscribe_int)(uint8_t* bit_no) {
    kbc_hook_id = *bit_no;
    return sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbc_hook_id);
}

int (kbc_unsubscribe_int)() {
    return sys_irqrmpolicy(&kbc_hook_id);
}

void (kbc_ih)() {
    uint8_t status = 0;
    while (1) {
        if (util_sys_inb(KBC_STAT_REG, &status) != 0) data = 0;
        if (status & KBC_OBF_FULL) {
            if (util_sys_inb(KBC_OUT_BUF, &data) != 0) data = 0;
            if (status & (KBC_PAR_ERR | KBC_TO_ERR)) data = 0;
            break;
        }
    }
}

int (kbc_read_cmd_byte)(uint8_t* byte) {
    uint8_t status = 0;
    int tries = 5;
    while (tries > 0) {
        if (util_sys_inb(KBC_STAT_REG, &status) != 0) return 1;
        if (status & KBC_IBF_FULL) {
            tries--;
            continue;
        }
        else if (sys_outb(KBC_CMD_REG, KBC_READ_CMD) != 0) return 1;
        tickdelay(micros_to_ticks(DELAY_US));
        break;
    }
    tries = 5;
    while (tries > 0) {
        if (util_sys_inb(KBC_STAT_REG, &status) != 0) return 1;
        if (status & KBC_OBF_FULL) {
            if (util_sys_inb(KBC_OUT_BUF, byte) != 0) return 1;
            if (!(status & (KBC_PAR_ERR | KBC_TO_ERR))) return 0;
            else return -1;
        }
        tickdelay(micros_to_ticks(DELAY_US));
        tries--;
    }
    return 0;
}

int (kbc_write_cmd_byte)(uint8_t cmd) {
    uint8_t status = 0;
    int tries = 5;
    while (tries > 0) {
        if (util_sys_inb(KBC_STAT_REG, &status) != 0) return 1;
        if (status & KBC_IBF_FULL) {
           tries--;
           continue;
        }
        else if (sys_outb(KBC_CMD_REG, KBC_WRITE_CMD) != 0) return 1;
        tickdelay(micros_to_ticks(DELAY_US));
        break;
    }
    tries = 5;
    while (tries > 0) {
        if (util_sys_inb(KBC_STAT_REG, &status) != 0) return 1;
        if (!(status & KBC_IBF_FULL)) return sys_outb(KBC_CMD_REG, cmd);
        tickdelay(micros_to_ticks(DELAY_US));
        tries--;
    }
    return 0;
}
