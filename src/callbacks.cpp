#include "tracer.h"
#include "callbacks.h"
#include "TracingState.h"
#include "utilities.h"
#include <instrumentr/instrumentr.h>
#include "utilities.h"
#include "parser.h"

void handle_package(TracingState* tracing_state,
                    const std::string& package_name) {
    std::string sig_file = tracing_state->get_cache_file(package_name);

    if (file_exists(sig_file)) {
        Rprintf("Reading strictness signature file '%s' for package '%s'\n",
                sig_file.c_str(),
                package_name.c_str());
        Package* package = new Package(package_name);
        parse_file(package, sig_file);
        package->apply();
        tracing_state->update_status(package);
        delete package;
    } else {
        Rprintf(
            "Ignoring package '%s', missing strictness signature file '%s'\n",
            package_name.c_str(),
            sig_file.c_str());
    }
}

void package_load_callback(instrumentr_tracer_t tracer,
                           instrumentr_callback_t callback,
                           instrumentr_state_t state,
                           instrumentr_application_t application,
                           instrumentr_environment_t environment) {
    TracingState* tracing_state = strictr_tracer_get_tracing_state(state);

    const char* package_name = instrumentr_environment_get_name(environment);

    if (package_name == nullptr) {
        Rf_error("unexpected null pointer for environment name");
    }

    handle_package(tracing_state, package_name);
}

void tracing_entry_callback(instrumentr_tracer_t tracer,
                            instrumentr_callback_t callback,
                            instrumentr_state_t state,
                            instrumentr_application_t application) {
    TracingState* tracing_state = strictr_tracer_get_tracing_state(state);

    SEXP package_names = R_lsInternal(R_NamespaceRegistry, TRUE);

    for (int i = 0; i < Rf_length(package_names); ++i) {
        const std::string package_name = CHAR(STRING_ELT(package_names, i));

        handle_package(tracing_state, package_name);
    }
}

void tracing_exit_callback(instrumentr_tracer_t tracer,
                           instrumentr_callback_t callback,
                           instrumentr_state_t state,
                           instrumentr_application_t application) {
    TracingState* tracing_state = strictr_tracer_get_tracing_state(state);

    instrumentr_state_insert(
        state, "application", tracing_state->get_status(), 1);

    strictr_tracer_remove_tracing_state(state);
}
