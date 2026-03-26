#ifndef ADQ_ACK_H
#define ADQ_ACK_H

#include <stdint.h>

#include "adq_protocol.h"

#define ADQ_ACK_PAYLOAD_SIZE 3u

typedef enum {
    ADQ_NACK_REASON_NONE = 0,
    ADQ_NACK_REASON_CRC = 1,
    ADQ_NACK_REASON_SEQ_GAP = 2,
    ADQ_NACK_REASON_DUPLICATE = 3,
    ADQ_NACK_REASON_SLOT_VIOLATION = 4,
    ADQ_NACK_REASON_TIMEOUT = 5,
    ADQ_NACK_REASON_BUSY = 6
} adq_nack_reason_t;

typedef enum {
    ADQ_TXN_IDLE = 0,
    ADQ_TXN_WAIT_FEEDBACK,
    ADQ_TXN_RETRY_READY,
    ADQ_TXN_COMPLETED,
    ADQ_TXN_FAILED
} adq_txn_state_t;

typedef struct {
    adq_txn_state_t state;
    uint16_t pending_seq;
    uint8_t attempts;
    uint8_t max_retries;
    uint32_t ack_timeout_us;
    uint64_t last_tx_us;
} adq_txn_manager_t;

int adq_ack_build_frame(uint16_t node_id,
                        uint16_t seq,
                        uint64_t timestamp_us,
                        int is_ack,
                        adq_nack_reason_t reason,
                        adq_frame_t* out);
int adq_ack_parse_frame(const adq_frame_t* frame,
                        uint16_t* seq,
                        int* is_ack,
                        adq_nack_reason_t* reason);

void adq_txn_manager_init(adq_txn_manager_t* manager, uint32_t ack_timeout_us, uint8_t max_retries);
void adq_txn_manager_reset(adq_txn_manager_t* manager);
int adq_txn_manager_on_tx(adq_txn_manager_t* manager, uint16_t seq, uint64_t now_us);
int adq_txn_manager_on_feedback(adq_txn_manager_t* manager, const adq_frame_t* feedback_frame);
int adq_txn_manager_poll(adq_txn_manager_t* manager, uint64_t now_us, int* retry_ready, int* failed);

#endif