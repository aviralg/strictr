#ifndef STRICTR_TRACING_STATE_H
#define STRICTR_TRACING_STATE_H

#include <string>
#include "Rincludes.h"

class TracingState {
  public:
    TracingState(const std::string& cache_dir): cache_dir_(cache_dir) {
    }

    std::string get_cache_file(const std::string& package_name) {
        return cache_dir_ + "/" + package_name;
    }

  private:
    std::string cache_dir_;
};

SEXP wrap_tracing_state(TracingState* tracing_state);

TracingState* unwrap_tracing_state(SEXP r_tracing_state);

#endif /* STRICTR_TRACING_STATE_H */
