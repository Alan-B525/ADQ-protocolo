# Hardware Workspace

Este directorio se alinea al enfoque vigente de ADQ v1.

## Baseline de hardware

- SoC objetivo: nRF52840 para nodo y base.
- Banda objetivo: 2.4 GHz.
- Alcance de validacion v1: 100 m LOS.

## Contenido esperado

- esquematico de nodo
- esquematico de base
- BOM
- notas de layout RF
- checklist de bring-up

## Recomendaciones de diseno

- Separar rutas analogico/RF/digital.
- Incluir test points para SWD/UART/consumo.
- Reservar puntos para calibracion de front-end strain gauge.
- Priorizar estabilidad de alimentacion y referencia de reloj.

## Criterio de avance

No pasar a validacion de campo sin checklist de bring-up completo y mediciones basicas de radio en banco.
