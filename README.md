# ADQ-protocolo

Sistema de adquisicion inalambrica para nodos remotos con sensores strain gauge y una basestation central.

**👀 Project Status?** See [PROJECT_STATUS.md](PROJECT_STATUS.md) — **Single source of truth** for what's done, blockers, and what's next.

**📊 Update Status:** At end of work day, run `/update-project-status` to automatically update the file.

## Objetivo de la v1

Construir una primera version funcional inspirada en principios tipo LXRS:

- Topologia estrella (nodo -> basestation).
- Sincronizacion temporal por beacon.
- Fiabilidad por ACK/NACK y retransmision.
- Buffer local para recuperacion ante cortes de enlace.
- Telemetria de diagnostico del enlace y del nodo.

## Estructura del repositorio

- `docs/`: arquitectura, protocolo, pruebas, ADRs e investigacion.
- `firmware/common/`: librerias compartidas (frames, CRC, ring buffer, sincronizacion).
- `firmware/node/`: app del nodo remoto.
- `firmware/base/`: app de basestation.
- `tools/host-cli/`: utilidades de configuracion/captura desde PC.
- `tools/rf-lab/`: utilidades de laboratorio RF.
- `tests/unit/`: pruebas unitarias de componentes de protocolo.
- `tests/sim/`: simulaciones funcionales de perdida/reintento.
- `hardware/`: artefactos de hardware (BOM, notas, esquematicos).

## Alcance de implementacion inicial

Esta base incluye:

- Documentacion tecnica v1.
- Esqueleto de firmware portable en C.
- CLI minima para parse de tramas en host.
- Prueba unitaria inicial de CRC.

No incluye aun:

- Driver RF especifico de chip.
- Driver ADC especifico.
- Integracion con RTOS.
- Cifrado completo de payload.

## Requisitos y validación del entorno

La build del firmware usa únicamente fuentes C99 propios y CMake, por lo que solo necesitas:

- `cmake` >= 3.16 (el `cmake_minimum_required` lo exige).
- Un compilador C compatible (MSVC con el paquete "Desktop development with C++" de Visual Studio Build Tools, o `gcc`/`clang` desde MinGW/MSYS2).
- Opcional: `python` 3.8+ si quieres ejecutar las utilidades de `tools/host-cli`.

Antes de ejecutar la generación, abre una terminal preparada para el compilador que vayas a usar: por ejemplo un "Developer Command Prompt" de Visual Studio/Build Tools o un PowerShell/Bash donde `gcc` esté en el `PATH`.

Comprueba que las herramientas están accesibles con:

```powershell
cmake --version
cl /?
# o, si usas MinGW:
gcc --version
python --version
```

Si algún comando falla, instala la herramienta correspondiente o corrige tu `PATH` antes de continuar.

## Construcción y pruebas

Una vez la herramienta de compilación esté disponible, genera los artefactos host con:

```bash
cmake -S . -B build
cmake --build build
```

El target `ADQ_BUILD_TESTS` está habilitado por defecto (ON). Para ejecutar las pruebas unitarias del CRC usa:

```bash
ctest --test-dir build
```

Para regenerar el build con una herramienta diferente (por ejemplo `ninja`), puedes pasar `-G Ninja` a `cmake -S . -B build`.

## Utilidades auxiliares

El script `tools/host-cli/parse_frames.py` es una herramienta de Python 3 que solo usa la biblioteca estándar (`binascii`, `struct`, `sys`). Para parsear tramas desde tu PC ejecuta:

```bash
python tools/host-cli/parse_frames.py 010203...
```

Reemplaza `010203...` por la secuencia hexadecimal capturada. No necesitas dependencias adicionales.

## Siguientes hitos

1. Integrar HAL real de radio sub-GHz.
2. Integrar front-end ADC strain gauge.
3. Ejecutar pruebas RF de alcance (0.2, 1 y 2 km LOS).
4. Ajustar protocolo v0.1 segun resultados de campo.
