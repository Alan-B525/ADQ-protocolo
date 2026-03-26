# PROJECT STATUS - ADQ-PROTOCOLO

**Ultima actualizacion:** 2026-03-26  
**Version de referencia:** ADQ v1

---

## 1) Informacion breve del estado actual del proyecto

| Item | Estado actual |
|---|---|
| Direccion tecnica | Cerrada (nRF52840 + ESB-like + 100 m) |
| Progreso global | 55% |
| Estado general | Buen avance de firmware comun y MAC determinista |
| Bloqueador principal | Backend RF real nRF52840 pendiente |

Resumen:

- Ya no se sigue la ruta anterior; el baseline tecnico actual esta consolidado.
- El firmware comun tiene protocolo, parser, fragmentacion, scheduler y ACK/NACK manager.
- El mayor riesgo actual es integracion RF real y validacion de campo 10/50/100 m.

---

## 2) Que se hizo y como

Hecho:

- Perfil de enlace 100 m.
- HAL RF base (loopback).
- Fragmentacion/reensamble.
- Parser FSM de stream.
- Scheduler de slots y beacon payload.
- Gating de TX en nodo por ventana de slot.
- Gestor de transacciones ACK/NACK (timeout, retry, fallo).
- Tests unitarios para scheduler, slot, ack y transacciones.

Como:

- Implementacion en firmware/common, firmware/base y firmware/node.
- Contratos de protocolo documentados y alineados.
- Cobertura de pruebas unitarias ampliada para nueva logica MAC.

---

## 3) Que falta hacer

Pendiente prioritario:

- [ ] Integrar backend RF real nRF52840 en HAL.
- [ ] Cerrar flujo end-to-end real DATA_FRAME -> ACK/NACK con radio real.
- [ ] Implementar ventana de secuencia y anti-duplicados en base.
- [ ] Ejecutar pruebas 10 m, 50 m y 100 m con KPIs.

KPIs de salida v1:

- PER en objetivo acordado para 100 m.
- latencia media y p95 dentro de presupuesto.
- retries controlados sin perdida silenciosa.
- recovery funcional ante cortes temporales.

---

## 4) Informacion necesaria para continuar o preguntas a realizar

| Pregunta / dato | Impacto | Owner | Fecha limite |
|---|---|---|---|
| Config final de canal/bitrate/potencia | Alta | Firmware Lead | Inmediata |
| Umbral PER aceptable para gate v1 | Alta | Lead | Antes de campo 50 m |
| Presupuesto de latencia (media y p95) | Alta | Lead | Antes de campo 100 m |
| Cantidad objetivo de nodos simultaneos v1 | Media | Lead | Antes de validacion final |

---

## Preguntas para el programador (responder o consultar)

- Confirmamos set RF final para validar 100 m?
- Cual es el umbral exacto de PER para aprobar v1?
- Cerramos maximo de nodos para la campana inicial?
