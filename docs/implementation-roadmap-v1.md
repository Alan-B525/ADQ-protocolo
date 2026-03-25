# Implementation Roadmap v1

## Sprint 1

- Base de repositorio y documentacion tecnica.
- Librerias comunes: CRC, codec, ring buffer, time sync.
- App minima de nodo y basestation.

## Sprint 2

- HAL de radio real (chip seleccionado).
- Scheduler de slots TDMA.
- Telemetria minima de diagnostico.

## Sprint 3

- Integracion front-end strain gauge real.
- Pipeline de captura host y export.
- Pruebas bench y estabilidad.

## Sprint 4

- Pruebas de campo LOS 0.2/1/2 km.
- Ajuste de parametros RF y retransmision.
- Cierre de release v1.

## Sprint 5 (hardening)

- Parser de stream con maquina de estados y fuzz testing.
- Ventana de secuencia y politicas anti-duplicado.
- Telemetria de salud extendida y codigos de falla normalizados.
- Prueba de soak de 24 horas con reporte estandar.

## Sprint 6 (security + operations)

- PSK de red, anti-replay por frame counter, auth en join.
- Firma de firmware y base para secure boot.
- Runbooks de operacion de campo y troubleshooting.
- Plantilla de reporte de incidente con export automatizado.

## Sprint 7 (industrialization)

- DFT/DFM de PCB y plan de test de produccion.
- FMEA de nodo y basestation.
- Criterios de aceptacion de fabrica y trazabilidad de lote.
- Cierre de backlog v1.1 y plan de escalado de nodos.
