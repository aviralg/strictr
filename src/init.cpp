#include "Rincludes.h"
#include <stdlib.h> // for NULL
#include "callbacks.h"

extern "C" {

static const R_CallMethodDef callMethods[] = {
    {"r_strictr_initialize_strictr", (DL_FUNC) &r_strictr_initialize_strictr, 2},
    {"r_strictr_finalize_strictr", (DL_FUNC) &r_strictr_finalize_strictr, 0},
    {"r_strictr_package_load_callback", (DL_FUNC)& r_strictr_package_load_callback, 2},
    {NULL, NULL, 0}};

void R_init_strictr(DllInfo* dll) {
    R_registerRoutines(dll, NULL, callMethods, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
}
