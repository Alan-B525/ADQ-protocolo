# Arduino Integration Guide (alineado al enfoque actual)

## Proposito

Usar la libreria Arduino como entorno de aprendizaje/prototipo, manteniendo compatibilidad conceptual con el enfoque vigente del proyecto.

## Libreria disponible

Ruta:

- arduino/ADQProtocol

Incluye:

- codec
- CRC16
- ring buffer
- sincronizacion temporal basica
- ejemplos de nodo/base

## Flujo minimo con dos placas

1. Nodo: arduino/ADQProtocol/examples/NodeTxDemo/NodeTxDemo.ino
2. Base: arduino/ADQProtocol/examples/BaseRxDemo/BaseRxDemo.ino
3. Validar encode/decode y secuencia de mensajes.

## Importante

La capa RF final del proyecto es nRF52840 2.4 GHz ESB-like. Arduino aqui se usa para pruebas didacticas y validacion de logica, no como referencia final de stack RF.

## Recomendaciones

- Validar primero framing + CRC + secuencia por serial/loopback.
- Integrar despues ACK/NACK y politicas de retry.
- Mantener compatibilidad de campos con docs/protocol/protocol-v1.md.

## Limites de esta guia

- No define backend RF final.
- No reemplaza las pruebas de campo del firmware principal.
