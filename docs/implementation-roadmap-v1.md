# Implementation Roadmap v1

## Objetivo del roadmap

Alinear todo el trabajo al enfoque vigente: nRF52840 + 2.4 GHz ESB-like + MAC determinista + 100 m LOS.

## Fase A - Baseline de transporte (completada en gran parte)

- [x] Perfil de enlace 100 m.
- [x] HAL RF base en loopback.
- [x] Fragmentacion y reensamble.
- [x] Parser FSM de stream.

## Fase B - MAC determinista (completada en gran parte)

- [x] Scheduler de slots por nodo.
- [x] Beacon payload con parametros de slot.
- [x] Node slot gating.
- [x] Gestor de transacciones ACK/NACK con timeout y reintentos.

## Fase C - Integracion RF real nRF52840 (activa)

- [ ] Reemplazar loopback por backend RF real.
- [ ] Conectar IRQ/eventos de radio al pipeline de tx/rx.
- [ ] Exponer RSSI y metricas de enlace en tiempo real.

## Fase D - Validacion por etapas (activa)

- [ ] Banco: 2 nodos + 1 base en entorno controlado.
- [ ] Campo corto: 10 m.
- [ ] Campo medio: 50 m.
- [ ] Campo objetivo: 100 m.
- [ ] Cierre de evidencia con PER, latencia y retries.

## Fase E - Hardening inmediato

- [ ] Ventana de secuencia por nodo y anti-duplicados en base.
- [ ] Taxonomia final de motivos NACK.
- [ ] Politica de recovery y resync con criterios de entrada/salida.

## Criterio de salida v1

- Enlace estable a 100 m LOS.
- Sin perdida silenciosa.
- Evidencia reproducible de retry/recovery.
- Baseline de pruebas unitarias y de campo documentada.
