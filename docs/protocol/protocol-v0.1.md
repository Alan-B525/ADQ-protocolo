# Wireless Protocol v0.1 (Draft)

## Objetivo

Definir un protocolo ligero y portable para red estrella con sincronizacion por beacon, ACK/retransmision y telemetria de estado.

## Convenciones

- Endianness: little-endian.
- Integridad: CRC-16-CCITT en cada trama.
- Versionado: campo `proto_version` en header.

## Estructura comun de trama

| Campo | Bytes | Descripcion |
|---|---:|---|
| start | 1 | 0xA5 |
| proto_version | 1 | Version de protocolo |
| msg_type | 1 | Tipo de mensaje |
| flags | 1 | Bits de control |
| node_id | 2 | ID logico nodo |
| seq | 2 | Secuencia incremental |
| timestamp_us | 8 | Timestamp en microsegundos |
| payload_len | 2 | Longitud payload |
| payload | N | Datos por tipo |
| crc16 | 2 | CRC de header+payload |

## Tipos de mensaje (v0.1)

- `0x01` HELLO
- `0x02` JOIN_ACK
- `0x03` BEACON
- `0x04` CONFIG_SET
- `0x05` START_STREAM
- `0x06` STOP_STREAM
- `0x07` DATA_FRAME
- `0x08` ACK
- `0x09` NACK
- `0x0A` HEARTBEAT
- `0x0B` DIAG_REPORT

## DATA_FRAME payload (v0.1)

| Campo | Bytes | Descripcion |
|---|---:|---|
| sample_rate_hz | 2 | Tasa de muestreo |
| channel_mask | 2 | Canales activos |
| sample_count | 2 | Muestras en payload |
| battery_mv | 2 | Bateria en mV |
| rssi_dbm | 1 | RSSI ultimo enlace |
| status_bits | 1 | Estado ADC/radio |
| samples | M | Muestras empaquetadas |

## Fiabilidad

- Cada `DATA_FRAME` requiere `ACK` o `NACK`.
- `NACK` incluye codigo de causa.
- Reintentos maximos configurables (`max_retries`).
- Si excede reintentos, nodo retiene datos en buffer y pasa a `RECOVERY`.

## Sincronizacion temporal

- `BEACON` periodico desde basestation con `timestamp_us`.
- Nodo calcula offset local y drift estimado.
- Resincronizacion periodica cada `sync_interval_s`.

## Errores y codigos base

- `0x01` CRC_INVALID
- `0x02` UNKNOWN_MSG
- `0x03` BUFFER_OVERFLOW
- `0x04` SLOT_VIOLATION
- `0x05` INTERNAL_ERROR

## Compatibilidad futura

- Mantener campos fijos de header.
- Nuevos mensajes en `msg_type` sin romper parse existente.
- Opcional TLV en payload para extensiones v1.1+.
