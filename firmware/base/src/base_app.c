#include "base_app.h"

#include "adq_codec.h"

void base_init(base_stats_t* stats) {
    stats->rx_ok = 0;
    stats->rx_crc_error = 0;
    stats->rx_unknown = 0;
}

int base_process_bytes(base_stats_t* stats, const uint8_t* data, size_t len, adq_frame_t* out) {
    const int rc = adq_decode_frame(data, len, out);
    if (rc == 0) {
        stats->rx_ok++;
        return 0;
    }

    if (rc == -4) {
        stats->rx_crc_error++;
    } else {
        stats->rx_unknown++;
    }

    return rc;
}
