#include "tracer.h"
#include "callbacks.h"
#include "TracingState.h"
#include "utilities.h"
#include <instrumentr/instrumentr.h>
#include "utilities.h"
#include "StrictnessSignatureCache.h"

void package_load_callback(instrumentr_tracer_t tracer,
                           instrumentr_callback_t callback,
                           instrumentr_application_t application,
                           instrumentr_package_t package) {
    const char* name = instrumentr_package_get_name(package);

    TracingState* tracing_state = strictr_tracer_get_tracing_state(tracer);
    std::string sig_file = tracing_state->get_cache_file(name);

    if (file_exists(sig_file)) {
        Rprintf("Reading cache from '%s'\n", sig_file.c_str());
        StrictnessSignatureCache cache = parse_file(sig_file);
        cache.apply();
    } else {
        Rprintf(
            "Ignoring '%s', cannot find strictness signatures at location '%s'",
            name,
            sig_file.c_str());
    }
}
