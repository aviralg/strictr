#include "tracer.h"
#include "TracingState.h"
#include "callbacks.h"
#include "instrumentr.h"

void strictr_tracing_state_destroy(SEXP r_tracing_state) {
    void* pointer = instrumentr_r_externalptr_to_c_pointer(r_tracing_state);
    if (pointer == NULL) {
        instrumentr_log_error("tracing state is null");
    } else {
        TracingState* state = static_cast<TracingState*>(pointer);
        delete state;
        instrumentr_r_externalptr_clear(r_tracing_state);
    }
}

SEXP strictr_tracing_state_create(SEXP r_cache_dir) {
    const std::string cache_dir = CHAR(STRING_ELT(r_cache_dir, 0));
    TracingState* state = new TracingState(cache_dir);
    return instrumentr_c_pointer_to_r_externalptr(
        state, R_NilValue, R_NilValue, strictr_tracing_state_destroy);
}

TracingState* strictr_tracer_get_tracing_state(instrumentr_state_t state) {
    SEXP r_tracing_state =
        instrumentr_state_lookup(state, "tracing_state", R_NilValue);
    void* data = instrumentr_r_externalptr_to_c_pointer(r_tracing_state);
    return static_cast<TracingState*>(data);
}

void strictr_tracer_remove_tracing_state(instrumentr_state_t state) {
    instrumentr_state_erase(state, "tracing_state", 1);
}

SEXP r_strictr_tracer_create(SEXP r_cache_dir) {
    instrumentr_tracer_t tracer = instrumentr_tracer_create();

    instrumentr_callback_t callback =
        instrumentr_callback_create_from_c_function(
            (void*) (package_load_callback), INSTRUMENTR_EVENT_PACKAGE_LOAD);
    instrumentr_tracer_set_callback(tracer, callback);
    instrumentr_object_release(callback);

    callback = instrumentr_callback_create_from_c_function(
        (void*) (tracing_entry_callback), INSTRUMENTR_EVENT_TRACING_ENTRY);
    instrumentr_tracer_set_callback(tracer, callback);
    instrumentr_object_release(callback);

    callback = instrumentr_callback_create_from_c_function(
        (void*) (tracing_exit_callback), INSTRUMENTR_EVENT_TRACING_EXIT);
    instrumentr_tracer_set_callback(tracer, callback);
    instrumentr_object_release(callback);

    instrumentr_state_t state = instrumentr_tracer_get_state(tracer);

    instrumentr_state_insert(
        state, "tracing_state", strictr_tracing_state_create(r_cache_dir), 1);

    SEXP r_tracer = instrumentr_tracer_wrap(tracer);
    instrumentr_object_release(tracer);
    return r_tracer;
}
