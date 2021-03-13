#ifndef STRICTR_TRACING_STATE_H
#define STRICTR_TRACING_STATE_H

#include <string>
#include "Rincludes.h"
#include "utilities.h"
#include "Package.h"

class TracingState {
  public:
    TracingState(const std::string& cache_dir): cache_dir_(cache_dir) {
    }

    std::string get_cache_file(const std::string& package_name) {
        return cache_dir_ + "/" + package_name;
    }

    void update_status(const std::vector<std::string>& names,
                       const std::vector<bool>& signature);

    void update_status(Package* package) {
        package->get_status(packages_, levels_, names_, applied_);
    }

    SEXP get_status() {
        std::vector<SEXP> columns({PROTECT(character_vector_wrap(packages_)),
                                   PROTECT(integer_vector_wrap(levels_)),
                                   PROTECT(character_vector_wrap(names_)),
                                   PROTECT(logical_vector_wrap(applied_))});

        std::vector<std::string> names({"package", "level", "name", "applied"});

        SEXP df = create_data_frame(names, columns);

        UNPROTECT(4);

        return df;
    }

  private:
    std::string cache_dir_;
    std::vector<std::string> packages_;
    std::vector<int> levels_;
    std::vector<std::string> names_;
    std::vector<int> applied_;
};

SEXP wrap_tracing_state(TracingState* tracing_state);

TracingState* unwrap_tracing_state(SEXP r_tracing_state);

#endif /* STRICTR_TRACING_STATE_H */
