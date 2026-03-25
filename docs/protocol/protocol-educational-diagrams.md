# Diagramas Educativos del Protocolo

Este documento concentra diagramas para explicar de forma didactica como funciona el protocolo extremo a extremo.

## 1) Diagrama de bloques por capas


- El sistema esta dividido por capas, cada una con una responsabilidad.
- Si falla una capa, sabes donde depurar primero.

```mermaid
flowchart TB
  A[Capa de aplicacion\ncomandos y telemetria] --> B[Capa de protocolo\ntramas seq crc]
  B --> C[Capa MAC\nslots TDMA ACK NACK]
  C --> D[Capa PHY\nenlace RF subGHz]
  D --> E[Canal\ncondiciones de campo]
```

## 2) Módulos internos del Nodo


- El Node no solo mide; tambien empaqueta, guarda temporalmente y reintenta.

```mermaid
flowchart LR
  S[Frontend Strain Gauge] --> A[Adquisicion]
  A --> P[Constructor de paquetes]
  P --> Q[Buffer local]
  Q --> R[Radio TX]
  B[Sincronizacion de BEACON] --> T[Sincronizacion de tiempo]
  T --> P
  R --> X[Manejador ACK]
  X --> Q
```

## 3) Módulos internos de la Estación Base


- La Base Station valida, ordena y responde, no es solo un receptor pasivo.

```mermaid
flowchart LR
  R[Radio RX] --> V[Frame Validator]
  V --> D[Deduplicacion y ventana de secuencia]
  D --> A[Motor ACK NACK]
  D --> U[Subida por USB]
  U --> H[CLI de Host]
  C[Planificador de BEACON] --> R
```

## 4) Ciclo de unión


- Antes de medir, el Node debe unirse y sincronizarse.

```mermaid
sequenceDiagram
  participant N as Nodo
  participant B as Base

    N->>B: HELLO
    B-->>N: JOIN_ACK(node_id, slot)
    B-)N: BEACON(timestamp)
  N->>N: bloquear sincronizacion
```

## 5) Flujo de streaming y confiabilidad


- ACK/NACK es el mecanismo central para evitar perdidas silenciosas.

```mermaid
flowchart TD
  A[Muestra adquirida] --> B[Construir DATA_FRAME]
  B --> C[Enviar en slot]
  C -->|ACK| D[Confirmar y avanzar]
  C -->|NACK| E[Reintentar misma seq]
  E --> C
  C -->|Timeout| F[Incrementar contador de retry]
  F --> G{retry <= max?}
  G -->|si| C
  G -->|no| H[Entrar en RECOVERY]
```

## 6) Flujo de estados de recuperación


- Recovery no es error final; es un modo para volver a estado estable.

```mermaid
stateDiagram-v2
  [*] --> TRANSMISION
  TRANSMISION --> RECOVERY: max retries o perdida de sync
  RECOVERY --> RESYNC: beacon detectado
  RESYNC --> REPLAY: reenviar tramas en buffer
  REPLAY --> TRANSMISION: backlog vaciado
  RECOVERY --> ERROR: falla no recuperable
    ERROR --> [*]
```

## 7) Flujo de observabilidad operativa


- Sin metricas y logs no hay forma profesional de operar en campo.

```mermaid
flowchart LR
  A[Contadores de nodo] --> D[DIAG_REPORT]
  B[Contadores de base] --> D
  D --> E[Almacen de metricas host]
  E --> F[Dashboard o reporte de campo]
```

## 8) Ruta de endurecimiento de seguridad (v1 a v1.1)


- La seguridad se construye por etapas; en v1 suele ser basica y en v1.1 se endurece.

```mermaid
flowchart LR
  A[v1 CRC y seq] --> B[v1.1 autenticacion PSK]
  B --> C[frame counter anti-replay]
  C --> D[clave de sesion en join]
  D --> E[OTA con firmware firmado]
```
