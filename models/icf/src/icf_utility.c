#include "icf_utility.h"

static struct timespec g_timestamp;
void clock_get_hw_time(struct timespec *ts) {
    clock_gettime(CLOCK_MONOTONIC, ts);
}

double get_curr_time(void) {
    clock_get_hw_time(&g_timestamp);
    return g_timestamp.tv_sec + (double)g_timestamp.tv_nsec / (double)BILLION;
}

void debug_hex_dump(char *str, uint8_t *pSrcBufVA, uint32_t SrcBufLen) {
#if ICF_DEBUG_ENABLE
    uint8_t *pt;
    uint32_t x;
    pt = pSrcBufVA;
    debug_print("%s: %p, len = %d\n\r", str, pSrcBufVA, SrcBufLen);
    for (x = 0; x < SrcBufLen; x++) {
        if (x % 16 == 0) {
            debug_print("0x%04x : ", x);
        }
        debug_print("%02x ", ((uint8_t)pt[x]));
        if (x % 16 == 15) { debug_print("\n\r"); }
    }
    debug_print("\n\r");
#endif  // DEBUG_ENABLE
}

void hex_dump(char *str, uint8_t *pSrcBufVA, uint32_t SrcBufLen) {
    uint8_t *pt;
    uint32_t x;
    pt = pSrcBufVA;
    printf("%s: %p, len = %d\n\r", str, pSrcBufVA, SrcBufLen);
    for (x = 0; x < SrcBufLen; x++) {
        if (x % 16 == 0) {
            printf("0x%04x : ", x);
        }
        printf("%02x ", ((uint8_t)pt[x]));
        if (x % 16 == 15) { printf("\n\r"); }
    }
    printf("\n\r");
}

uint32_t crc32(uint32_t crc, const uint8_t *buf) {
    uint32_t crc_30_00;
    uint32_t crc_31;
    uint32_t buf_value;

    if (buf == 0) { return 0L; }

    buf_value =  *((uint32_t *) buf);
    crc_30_00 = ((crc >> 1) ^ (buf_value & 0x7fffffff));
    crc_31   = (crc & 0x1) ^
                ((crc & (0x1 << 4)) >> 4) ^
                ((crc & (0x1 << 8)) >> 8) ^
                ((crc & (0x1 << 12)) >> 12) ^
                ((crc & (0x1 << 17)) >> 17) ^
                ((crc & (0x1 << 20)) >> 20) ^
                ((crc & (0x1 << 23)) >> 23) ^
                ((crc & (0x1 << 28)) >> 28) ^
                ((buf_value & (0x1 << 31)) >> 31);

    return (((crc_31 << 31) & 0x80000000) | (crc_30_00 & 0x7fffffff));
}

uint32_t crc32_create(const uint8_t *buf, const uint32_t len) {
    uint32_t crc, total;
    crc = total = 0;
    while (total < len) {
        crc = crc32(crc, (buf + total));
        total += 4;
    }
    return crc;
}

uint32_t invert_crc32(uint32_t crc) {
    uint32_t crc_30_00;
    uint32_t crc_31;
    uint32_t crc_tmp;
    uint32_t inv_crc;
    uint8_t  *ucPoint;

    crc_30_00 = ((crc >> 1) & 0x7fffffff);
    crc_31  = (crc & 0x1) ^
               ((crc & (0x1 << 4)) >> 4) ^
               ((crc & (0x1 << 8)) >> 8) ^
               ((crc & (0x1 << 12)) >> 12) ^
               ((crc & (0x1 << 17)) >> 17) ^
               ((crc & (0x1 << 20)) >> 20) ^
               ((crc & (0x1 << 23)) >> 23) ^
               ((crc & (0x1 << 28)) >> 28);
    inv_crc = (crc_30_00 & 0x7fffffff) | ((crc_31 << 31) & 0x80000000);

    debug_print("  Invert_CRC crc32=0x%8x \n", inv_crc);
    ucPoint = (uint8_t *)&inv_crc;
    crc_tmp = crc32(crc, ucPoint);
    debug_print("  After Invert CRC==> 0x%x\n", crc_tmp);
    return inv_crc;
}

uint32_t crc_checker(uint32_t rx_crc, const uint8_t *buf, uint32_t size) {
    uint32_t crc = 0, total = 0;
    while (total < size) {
        crc = crc32(crc, (buf + total));
        total += 4;
    }
    // printf("crc 0x%x, rx_crc: 0x%x\n", crc, rx_crc);
    return (rx_crc == crc);
}

int get_arr_num(int arrary_size, int element_size) {
    if (arrary_size % element_size) {
        fprintf(stderr, "Arrary Size and Element size are not maching\n");
        return -1;
    }
    return (arrary_size/element_size);
}

int data_int16_to_uint8_bigendian(uint8_t *dest, int16_t *src) {
    dest[0] = (*src >> 8) & 0xFF;
    dest[1] = (*src & 0xFF);
    return 0;
}
