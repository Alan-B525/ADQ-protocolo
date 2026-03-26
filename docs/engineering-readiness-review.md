# Engineering Readiness Review

## Resumen ejecutivo

El proyecto ya cambio de rumbo y tiene direccion tecnica clara. Se debe cerrar ejecucion, no rediscutir arquitectura base.

Direccion cerrada:

- nRF52840
- 2.4 GHz ESB-like
- 100 m LOS
- MAC determinista con ACK/NACK y recovery

## Estado de madurez

- Baseline de protocolo: fuerte.
- Baseline de MAC determinista: fuerte.
- Backend RF real: pendiente (principal foco tecnico).
- Validacion de campo 10/50/100 m: pendiente.

## Riesgos prioritarios

1. Integracion RF real con temporizacion consistente.
2. Ajuste de parametros para sostener 100 m sin degradar latencia.
3. Cierre de politica de secuencia/duplicados en base.

## Mitigaciones en curso

- Parser FSM y fragmentacion ya implementados.
- Scheduler de slots y slot gating en nodo implementados.
- Gestor ACK/NACK con timeout/reintento implementado.

## Gate tecnico recomendado

Antes de declarar v1 listo, exigir:

- evidencia de 100 m LOS,
- KPIs de PER/latencia/retries,
- recovery verificado en corte temporal,
- set de pruebas unitarias ejecutable y estable.

## Prioridad de trabajo

1. Backend RF real nRF52840.
2. Pruebas de banco integradas.
3. Campana de campo por etapas.
4. Ajustes finos de secuencia/duplicados y motivo NACK.
