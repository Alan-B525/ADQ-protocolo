#include <ADQProtocol.h>

// Demo basico: espera bytes binarios por Serial y decodifica cuando junta una trama completa.
// En integracion real, estos bytes vendran del modulo RF.

static uint8_t rxBuf[300];
static size_t rxCount = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }
  Serial.println("BaseRxDemo start");
}

void loop() {
  while (Serial.available() > 0 && rxCount < sizeof(rxBuf)) {
    rxBuf[rxCount++] = (uint8_t)Serial.read();
  }

  // Intento de decode directo para demo simplificado.
  adq_frame_t frame;
  if (rxCount >= 20 && ADQProtocol::decode(rxBuf, rxCount, frame)) {
    Serial.print("RX ok node=");
    Serial.print(frame.node_id);
    Serial.print(" seq=");
    Serial.print(frame.seq);
    Serial.print(" type=");
    Serial.println(frame.msg_type, HEX);
    rxCount = 0;
  }

  if (rxCount >= sizeof(rxBuf)) {
    Serial.println("RX overflow, reset buffer");
    rxCount = 0;
  }
}
