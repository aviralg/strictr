#ifndef STRICTR_CALLBACKS_H
#define STRICTR_CALLBACKS_H

#include "Rincludes.h"

extern "C" {
SEXP r_strictr_initialize_strictr(SEXP r_log_filepath, SEXP r_cache_dir);
SEXP r_strictr_finalize_strictr();
SEXP r_strictr_package_load_callback(SEXP r_pkgname, SEXP r_lib);
}

#endif /* STRICTR_CALLBACKS_H  */
