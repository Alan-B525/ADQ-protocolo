#ifndef ADQ_PROTOCOL_H
#define ADQ_PROTOCOL_H

#include <stddef.h>
#include <stdint.h>

#define ADQ_FRAME_START 0xA5u
#define ADQ_PROTO_VERSION 0x01u
#define ADQ_MAX_PAYLOAD 240u

typedef enum {
    ADQ_MSG_HELLO = 0x01,
    ADQ_MSG_JOIN_ACK = 0x02,
    ADQ_MSG_BEACON = 0x03,
    ADQ_MSG_CONFIG_SET = 0x04,
    ADQ_MSG_START_STREAM = 0x05,
    ADQ_MSG_STOP_STREAM = 0x06,
    ADQ_MSG_DATA_FRAME = 0x07,
    ADQ_MSG_ACK = 0x08,
    ADQ_MSG_NACK = 0x09,
    ADQ_MSG_HEARTBEAT = 0x0A,
    ADQ_MSG_DIAG_REPORT = 0x0B
} adq_msg_type_t;

typedef struct {
    uint8_t start;
    uint8_t proto_version;
    uint8_t msg_type;
    uint8_t flags;
    uint16_t node_id;
    uint16_t seq;
    uint64_t timestamp_us;
    uint16_t payload_len;
    uint8_t payload[ADQ_MAX_PAYLOAD];
    uint16_t crc16;
} adq_frame_t;

#endif
