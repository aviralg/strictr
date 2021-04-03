#include "callbacks.h"
#include "utilities.h"
#include "parser.h"
#include "TracingState.h"

FILE* log_file = NULL;
TracingState* tracing_state = nullptr;

void handle_package(const std::string& package_name) {
    std::string sig_file = tracing_state->get_cache_file(package_name);

    if (file_exists(sig_file)) {
        fprintf(log_file,
                "Reading strictness signature file '%s' for package '%s'\n",
                sig_file.c_str(),
                package_name.c_str());
        Package* package = new Package(package_name);
        parse_file(package, sig_file);
        package->apply(log_file);
        tracing_state->update_status(package);
        delete package;
    } else {
        fprintf(
            log_file,
            "Ignoring package '%s', missing strictness signature file '%s'\n",
            package_name.c_str(),
            sig_file.c_str());
    }
}

SEXP r_strictr_package_load_callback(SEXP r_pkgname, SEXP r_lib) {
    std::string package_name = CHAR(STRING_ELT(r_pkgname, 0));
    handle_package(package_name);
    return R_NilValue;
}

SEXP r_strictr_initialize_strictr(SEXP r_log_filepath, SEXP r_cache_dir) {
    const char* log_filepath = CHAR(STRING_ELT(r_log_filepath, 0));
    log_file = fopen(log_filepath, "w");

    if (log_file == NULL) {
        Rf_error("cannot opent file %s for writing logs", log_filepath);
    }

    const std::string cache_dir = CHAR(STRING_ELT(r_cache_dir, 0));
    tracing_state = new TracingState(cache_dir);

    SEXP package_names = R_lsInternal(R_NamespaceRegistry, TRUE);

    for (int i = 0; i < Rf_length(package_names); ++i) {
        const std::string package_name = CHAR(STRING_ELT(package_names, i));

        handle_package(package_name);
    }

    return R_NilValue;
}

SEXP r_strictr_finalize_strictr() {
    SEXP r_df = tracing_state->get_status();
    delete tracing_state;
    fclose(log_file);
    return r_df;
}
