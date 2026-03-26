# ADQ v1 vs LXRS (comparativa vigente)

## 1) Posicion de ADQ por capas

ADQ v1 opera como stack de control sobre radio:

- L1: PHY 2.4 GHz del nRF52840.
- L2: MAC determinista (beacon, slot, ACK/NACK, retry).
- L2.5: fragmentacion/reensamble.
- L7: mensajes de aplicacion y diagnostico.

## 2) Que se adopta de la inspiracion LXRS

Se adoptan principios de ingenieria:

- sincronizacion temporal,
- confiabilidad de entrega,
- continuidad operativa bajo perdida parcial.

No se replica implementacion propietaria.

## 3) Diferencia practica clave

| Tema | ADQ v1 | LXRS |
|---|---|---|
| Control de diseno | Total en repo propio | Cerrado por fabricante |
| Flexibilidad | Alta para adaptar payload/MAC | Alta dentro del ecosistema LXRS |
| Riesgo inicial | Mayor (se implementa stack propio) | Menor (producto maduro) |
| Costo de evolucion | Menor licencia, mayor ingenieria | Mayor dependencia de plataforma |

## 4) Decision vigente del proyecto

ADQ v1 mantiene enfoque propio sobre nRF52840 con objetivo 100 m LOS y validacion por etapas 10/50/100 m.

## 5) Criterio de exito

- estabilidad de enlace en 100 m,
- retries controlados,
- recovery funcional,
- trazabilidad por seq/rssi/latencia.
