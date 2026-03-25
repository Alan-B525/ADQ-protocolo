# System Architecture v1

## Contexto

El proyecto reemplaza enlaces cableados no viables (1-2 km) por una red inalambrica para adquisicion de strain gauge.

## Requisitos base asumidos para v1

- Alcance objetivo: 1-2 km en condiciones mayormente LOS.
- Topologia: estrella.
- Sincronizacion temporal: objetivo de orden 1 ms.
- Latencia objetivo: 100-500 ms.
- Interfaz de basestation: USB CDC.
- Prioridad: rapidez de implementacion para pruebas con base tecnica robusta.

## Bloques del sistema

### 1) Nodo remoto

- MCU de bajo consumo.
- Front-end strain gauge (ADC delta-sigma + PGA + excitacion puente).
- Radio sub-GHz.
- Buffer local (ring buffer persistente en memoria no volatil).
- Modulos firmware:
  - scheduler de adquisicion
  - sincronizacion temporal
  - empaquetado de tramas
  - reintento por ACK
  - diagnostico

### 2) Basestation

- MCU con radio sub-GHz.
- Scheduler de beacon.
- Asignador de slots TDMA por nodo.
- Colector de tramas y verificador de integridad.
- Forward por USB CDC a host.

### 3) Host

- CLI para configuracion, captura y exportacion.
- Parser de tramas con validacion de CRC.
- Registro de metricas de red.

## Flujo operativo

1. Basestation emite beacon periodico con marca temporal.
2. Nodo sincroniza reloj local y adquiere muestras.
3. Nodo arma DATA_FRAME con secuencia y timestamp.
4. Nodo transmite en su slot asignado.
5. Basestation responde ACK/NACK.
6. Nodo confirma avance de buffer o retransmite.

## Estados del nodo

- `BOOT`
- `DISCOVERY`
- `JOINED`
- `SYNCED`
- `STREAMING`
- `RECOVERY`
- `ERROR`

## Criterios de calidad v1

- Integridad de payload por CRC.
- Reintentos acotados y medidos.
- No perdida silenciosa de datos (todo drop debe quedar contabilizado).
- Trazabilidad de decisiones en ADR.
