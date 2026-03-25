#include <stdint.h>
#include <stdio.h>

#include "adq_crc16.h"

int main(void) {
    const uint8_t input[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    const uint16_t expected = 0x29B1u;
    const uint16_t got = adq_crc16_ccitt(input, sizeof(input));

    if (got != expected) {
        printf("crc mismatch got=0x%04X expected=0x%04X\n", got, expected);
        return 1;
    }

    printf("crc test ok\n");
    return 0;
}
