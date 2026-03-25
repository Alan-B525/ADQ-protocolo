# Test Plan v1

## Objetivo

Verificar funcionalidad, robustez y desempeno base del sistema inalambrico v1.

## 1. Pruebas unitarias

- CRC16: vectores conocidos.
- Codec de tramas: serialize/deserialize y casos invalidos.
- Ring buffer: overflow controlado y orden FIFO.
- Time sync: convergencia de offset y drift sintetico.

## 2. Pruebas de integracion (bench)

- Join de nodo y asignacion de ID.
- Inicio/parada de stream.
- ACK/NACK y retransmision.
- Recuperacion tras perdida temporal de enlace.

## 3. Pruebas de estabilidad

- Corrida de 8 horas con 2 nodos minimos.
- Registro de:
  - dropped frames
  - retransmisiones
  - drift temporal
  - uso de buffer

## 4. Pruebas de campo LOS

- Distancias: 0.2 km, 1 km, 2 km.
- KPIs:
  - PER (packet error rate)
  - RSSI por nodo
  - latencia extremo a extremo
  - continuidad de stream

## 5. Criterios de aceptacion v1

- Parser sin fallos con tramas validas e invalidas controladas.
- Reconexion automatica tras perdida de enlace.
- Sincronizacion dentro de objetivo operativo definido.
- Registro claro de metricas y eventos de error.
