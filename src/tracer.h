#ifndef STRICTR_TRACER_H
#define STRICTR_TRACER_H

#include "Rincludes.h"
#include "TracingState.h"
#include <instrumentr/instrumentr.h>

extern "C" {
SEXP r_strictr_tracer_create(SEXP r_cache_dir);

TracingState* strictr_tracer_get_tracing_state(instrumentr_state_t state);
void strictr_tracer_remove_tracing_state(instrumentr_state_t state);
}

#endif /* STRICTR_TRACER_H */
