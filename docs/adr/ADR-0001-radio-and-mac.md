# ADR-0001: Radio y estrategia MAC para v1

- Estado: Aceptado
- Fecha: 2026-03-25

## Contexto

Se necesita una v1 funcional para pruebas de campo a 1-2 km LOS con nodos de strain gauge. La prioridad actual es tiempo de implementacion y robustez basica.

## Decision

- Usar arquitectura de red estrella.
- Diseñar protocolo con sincronizacion por beacon.
- Usar esquema MAC TDMA simple con ACK/NACK.
- Mantener buffer local y reintento para mejorar continuidad de datos.

## Justificacion

- Reduce colisiones en red multi-nodo.
- Hace predecible la ocupacion temporal del canal.
- Permite telemetria clara de perdida/reintento.
- Facilita evolucion a una version mas avanzada sin romper base.

## Consecuencias

- Requiere scheduler de slots en basestation.
- Requiere control de tiempo razonable en nodo.
- No cubre mesh/repetidores en v1.
