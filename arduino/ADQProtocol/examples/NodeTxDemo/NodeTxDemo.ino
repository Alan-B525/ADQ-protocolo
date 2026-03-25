#include <ADQProtocol.h>

static uint16_t seqNum = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }

  Serial.println("NodeTxDemo start");
}

void loop() {
  adq_frame_t frame;
  uint8_t encoded[300];
  size_t encodedLen = 0;

  memset(&frame, 0, sizeof(frame));
  frame.start = ADQ_FRAME_START;
  frame.proto_version = ADQ_PROTO_VERSION;
  frame.msg_type = ADQ_MSG_DATA_FRAME;
  frame.flags = 0;
  frame.node_id = 0x0101;
  frame.seq = seqNum++;
  frame.timestamp_us = micros();

  // Payload de ejemplo: sample_rate=100Hz, channel_mask=1, sample_count=1, sample=123
  frame.payload[0] = 100;
  frame.payload[1] = 0;
  frame.payload[2] = 1;
  frame.payload[3] = 0;
  frame.payload[4] = 1;
  frame.payload[5] = 0;
  frame.payload[6] = 0x7C; // 3700 mV
  frame.payload[7] = 0x0E;
  frame.payload[8] = (uint8_t)-70;
  frame.payload[9] = 0;
  frame.payload[10] = 123;
  frame.payload[11] = 0;
  frame.payload_len = 12;

  if (ADQProtocol::encode(frame, encoded, sizeof(encoded), encodedLen)) {
    // Para demo, envio por Serial en HEX. Reemplaza por tu radio real.
    for (size_t i = 0; i < encodedLen; ++i) {
      if (encoded[i] < 16) {
        Serial.print('0');
      }
      Serial.print(encoded[i], HEX);
    }
    Serial.println();
  } else {
    Serial.println("encode fail");
  }

  delay(500);
}
