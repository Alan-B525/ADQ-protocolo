# Protocolo ADQ v1 (especificacion vigente)

## Alcance de esta especificacion

Esta especificacion define el contrato activo entre nodo y base para el enfoque actual:

- red estrella
- sincronizacion por beacon
- slots deterministas
- ACK/NACK con retry
- observabilidad de enlace

## Formato de trama comun

| Campo | Bytes | Descripcion |
|---|---:|---|
| start | 1 | 0xA5 |
| proto_version | 1 | Version de protocolo |
| msg_type | 1 | Tipo de mensaje |
| flags | 1 | Control y estado |
| node_id | 2 | ID logico de nodo |
| seq | 2 | Secuencia incremental |
| timestamp_us | 8 | Marca temporal |
| payload_len | 2 | Longitud de payload |
| payload | N | Datos del mensaje |
| crc16 | 2 | CRC-16-CCITT |

## Tipos de mensaje

- 0x01 HELLO
- 0x02 JOIN_ACK
- 0x03 BEACON
- 0x04 CONFIG_SET
- 0x05 START_STREAM
- 0x06 STOP_STREAM
- 0x07 DATA_FRAME
- 0x08 ACK
- 0x09 NACK
- 0x0A HEARTBEAT
- 0x0B DIAG_REPORT

## Payload de BEACON (asignacion de slot)

Tamano actual: 11 bytes.

| Byte(s) | Campo |
|---|---|
| 0 | version beacon payload |
| 1 | slot_index |
| 2..5 | beacon_interval_us |
| 6..7 | slot_us |
| 8..9 | guard_us |
| 10 | max_nodes |

## Payload de ACK/NACK

Tamano actual: 3 bytes.

| Byte(s) | Campo |
|---|---|
| 0..1 | seq referenciado |
| 2 | reason (0 en ACK, codigo en NACK) |

Motivos NACK usados por v1:

- 1 CRC
- 2 SEQ_GAP
- 3 DUPLICATE
- 4 SLOT_VIOLATION
- 5 TIMEOUT
- 6 BUSY

## Ciclo de envio confiable

1. Nodo transmite DATA_FRAME en su slot.
2. Nodo entra en espera de feedback.
3. Si llega ACK del mismo seq: transaccion completada.
4. Si llega NACK del mismo seq: marcar retry.
5. Si vence timeout sin feedback: marcar retry.
6. Si supera max_retries: transicion a recovery.

## Reglas operativas

- No transmitir fuera de slot valido.
- No aceptar tramas con CRC invalido.
- Registrar razon de fallo siempre que haya NACK o timeout.
- Mantener trazabilidad por node_id + seq + timestamp.

## DATA_FRAME (payload de aplicacion)

El payload actual incluye metadata de adquisicion y muestras codificadas. La estructura exacta puede evolucionar sin romper el header comun, siempre que se mantenga compatibilidad por version.

## Compatibilidad

- Endianness: little-endian.
- Extensiones: por tipo de mensaje y version.
- Cambios incompatibles: requieren incremento de version de protocolo.

## Regla de mantenimiento

Si se modifica cualquier comportamiento de protocolo, actualizar en el mismo cambio:

1. este documento,
2. tests unitarios relacionados,
3. diagramas de docs/protocol/protocol-educational-diagrams.md.
