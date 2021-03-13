#ifndef STRICTR_CALLBACKS_H
#define STRICTR_CALLBACKS_H

#include <instrumentr/instrumentr.h>

void package_load_callback(instrumentr_tracer_t tracer,
                           instrumentr_callback_t callback,
                           instrumentr_state_t state,
                           instrumentr_application_t application,
                           instrumentr_package_t package);

void tracing_entry_callback(instrumentr_tracer_t tracer,
                            instrumentr_callback_t callback,
                            instrumentr_state_t state,
                            instrumentr_application_t application);

void tracing_exit_callback(instrumentr_tracer_t tracer,
                           instrumentr_callback_t callback,
                           instrumentr_state_t state,
                           instrumentr_application_t application);

#endif /* STRICTR_CALLBACKS_H  */
