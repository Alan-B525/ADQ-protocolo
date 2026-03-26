# ADQ-protocolo

Sistema de adquisicion inalambrica para nodos remotos con strain gauge y una base central.

Este repositorio ya no sigue el enfoque anterior de largo alcance ni rutas alternativas. La direccion oficial es:

- Radio: nRF52840 (2.4 GHz)
- Enfoque MAC: propietario ESB-like determinista
- Alcance objetivo v1: hasta 100 m LOS
- Prioridad: confiabilidad y trazabilidad (no throughput maximo)

Ver estado operativo en PROJECT_STATUS.md.

## Enfoque actual (version vigente)

La version vigente se centra en 4 pilares:

1. Sincronizacion por beacon.
2. Slots deterministas para evitar colisiones.
3. Entrega confiable por ACK/NACK + retry con presupuesto controlado.
4. Integridad y observabilidad (CRC, secuencia, metricas de enlace).

## Estado tecnico actual del firmware

Implementado en firmware comun:

- Codec y CRC16
- Ring buffer
- Time sync
- Perfil de enlace 100 m
- HAL RF base (loopback)
- Fragmentacion y reensamble
- Parser FSM de stream
- Scheduler MAC (beacon + slots)
- Gestor de transacciones ACK/NACK con timeout y reintentos

Implementado en apps:

- Base: registro de nodos, generacion de beacon, procesamiento de stream y generacion de feedback ACK/NACK.
- Nodo: aplicacion de beacon, gating por slot, ciclo de tx/retry/recovery.

## Estructura del repositorio

- docs/: arquitectura, protocolo, pruebas, ADR, roadmap.
- firmware/common/: modulos compartidos de protocolo y MAC.
- firmware/node/: logica de nodo.
- firmware/base/: logica de base.
- tests/unit/: pruebas unitarias.
- tests/sim/: simulaciones funcionales.
- tools/host-cli/: utilidades de parseo de tramas.
- tools/rf-lab/: utilidades para pruebas RF en campo/lab.
- arduino/ADQProtocol/: libreria educativa/prototipo.

## Build y pruebas

Requisitos:

- CMake >= 3.16
- Compilador C99 (MSVC o GCC/Clang)
- Python 3.8+ (solo para utilidades host)

Comandos:

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

## Roadmap inmediato

1. Cerrar backend RF real nRF52840 en HAL.
2. Integrar flujo completo TX->ACK/NACK sobre backend real.
3. Ejecutar validacion por etapas 10 m, 50 m, 100 m.
4. Consolidar criterio de salida v1 con PER/latencia/recovery.

## Documentacion recomendada

1. docs/architecture/system-architecture-v1.md
2. docs/protocol/protocol-v1.md
3. docs/testing/test-plan-v1.md
4. docs/implementation-roadmap-v1.md
5. docs/adr/ADR-0001-radio-and-mac.md
