#ifndef STRICTR_PACKAGE_H
#define STRICTR_PACKAGE_H

#include "Function.h"
#include "Scope.h"
#include "Rincludes.h"

class Package: public Scope {
  public:
    Package(const std::string& name): Scope(name) {
    }

    Package(): Package("unknown") {
    }

    void apply(FILE* log_file) {
        std::string prefix = "█";

        fprintf(log_file, "%s %s\n", prefix.c_str(), name_.c_str());

        SEXP r_namespace = PROTECT(get_namespace_());

        int index = 0;

        int size = functions_.size();

        for (auto iter: functions_) {
            Function* function = iter.second;
            const std::string& fun_name = function->get_name();

            SEXP r_name = PROTECT(Rf_install(fun_name.c_str()));

            SEXP r_closure = PROTECT(Rf_findVarInFrame(r_namespace, r_name));

            if (r_closure == R_UnboundValue) {
                UNPROTECT(2);

                continue;
                // NOTE: ignore functions not found
                // Rf_error(
                //     "cannot find function '%s' in namespace of package '%s'",
                //     fun_name.c_str(),
                //     name_.c_str());
            }

            if (TYPEOF(r_closure) == PROMSXP) {
                r_closure = Rf_eval(r_closure, R_GlobalEnv);
            }

            if (TYPEOF(r_closure) == CLOSXP) {
                function->apply(log_file,
                                r_name,
                                r_namespace,
                                r_closure,
                                0,
                                index == size - 1);
            } else {
                // NOTE: ignore functions not found
                // Rf_error(
                //    "'%s' not bound to a closure in namespace of package
                //    '%s'", fun_name.c_str(), name_.c_str());
            }

            UNPROTECT(2);

            ++index;
        }

        UNPROTECT(1);
    }

    void get_status(std::vector<std::string>& packages,
                    std::vector<int>& levels,
                    std::vector<std::string>& names,
                    std::vector<int>& applied) {
        int old_size = levels.size();

        for (auto& iter: functions_) {
            iter.second->get_status(0, "", levels, names, applied);
        }

        int new_size = levels.size();

        for (int i = old_size; i < new_size; ++i) {
            packages.push_back(name_);
        }
    }

  private:
    SEXP get_namespace_() {
        SEXP r_namespace =
            Rf_findVarInFrame(R_NamespaceRegistry, Rf_install(name_.c_str()));

        if (r_namespace == R_UnboundValue) {
            Rf_error("cannot find namespace for package '%s'", name_.c_str());
        }

        return r_namespace;
    }
};

#endif /* STRICTR_PACKAGE_H */
