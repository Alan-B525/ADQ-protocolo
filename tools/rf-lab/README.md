# RF Lab Tools

Herramientas y procedimientos para medir el comportamiento del enlace RF de ADQ v1.

## Objetivo

Generar evidencia reproducible de calidad de enlace en 10 m, 50 m y 100 m.

## KPI minimos

- PER
- RSSI
- latencia media y p95
- retries por frame
- recoveries por corrida

## Flujo recomendado

1. Configurar perfil RF (canal, bitrate, potencia, timeout, max_retries).
2. Ejecutar corrida controlada por distancia.
3. Exportar resultados estructurados.
4. Comparar contra criterio de aceptacion v1.

## Nota

Este directorio se mantiene enfocado en el baseline vigente nRF52840 + 2.4 GHz ESB-like.
