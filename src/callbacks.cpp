#include "tracer.h"
#include "callbacks.h"
#include "TracingState.h"
#include "utilities.h"
#include <instrumentr/instrumentr.h>
#include "utilities.h"
#include "parser.h"

void package_load_callback(instrumentr_tracer_t tracer,
                           instrumentr_callback_t callback,
                           instrumentr_application_t application,
                           instrumentr_package_t package) {
    std::string package_name = instrumentr_package_get_name(package);

    TracingState* tracing_state = strictr_tracer_get_tracing_state(tracer);
    std::string sig_file = tracing_state->get_cache_file(package_name);

    if (file_exists(sig_file)) {
        Rprintf("Reading strictness signature file '%s' for package '%s'\n",
                sig_file.c_str(),
                package_name.c_str());
        PackageStrictnessSignature cache = parse_file(package_name, sig_file);
        cache.apply();
    } else {
        Rprintf(
            "Ignoring package '%s', missing strictness signature file '%s'\n",
            package_name.c_str(),
            sig_file.c_str());
    }
}
