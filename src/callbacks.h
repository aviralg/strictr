#ifndef STRICTR_CALLBACKS_H
#define STRICTR_CALLBACKS_H

#include <instrumentr/instrumentr.h>

void package_load_callback(instrumentr_tracer_t tracer,
                           instrumentr_callback_t callback,
                           instrumentr_application_t application,
                           instrumentr_package_t package);

#endif /* STRICTR_CALLBACKS_H  */
