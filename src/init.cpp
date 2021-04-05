#include "Rincludes.h"
#include <stdlib.h> // for NULL
#include "callbacks.h"
#include "utilities.h"

extern "C" {

static const R_CallMethodDef callMethods[] = {
    {"r_strictr_initialize_strictr", (DL_FUNC) &r_strictr_initialize_strictr, 2},
    {"r_strictr_finalize_strictr", (DL_FUNC) &r_strictr_finalize_strictr, 0},
    {"r_strictr_package_load_callback", (DL_FUNC)& r_strictr_package_load_callback, 2},
    {NULL, NULL, 0}};

void R_init_strictr(DllInfo* dll) {
    R_registerRoutines(dll, NULL, callMethods, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);

    R_MISSING_SYMBOL = Rf_install("missing");
    R_NOT_SYMBOL = Rf_install("!");
    R_LEFT_ASSIGN_SYMBOL = Rf_install("<-");
    R_LIST_SYMBOL = Rf_install("list");
    R_IF_SYMBOL = Rf_install("if");
    R_LEFT_BRACE_SYMBOL = Rf_install("{");

    R_PreserveObject(R_MISSING_SYMBOL);
    R_PreserveObject(R_NOT_SYMBOL);
    R_PreserveObject(R_LEFT_ASSIGN_SYMBOL);
    R_PreserveObject(R_LIST_SYMBOL);
    R_PreserveObject(R_IF_SYMBOL);
    R_PreserveObject(R_LEFT_BRACE_SYMBOL);
}

}
