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

    void apply() {
        std::string prefix = "█";

        Rprintf("%s %s\n", prefix.c_str(), name_.c_str());

        SEXP r_namespace = get_namespace_();

        int index = 0;

        int size = functions_.size();

        for (auto iter: functions_) {
            Function* function = iter.second;
            const std::string& fun_name = function->get_name();

            SEXP r_closure =
                Rf_findVarInFrame(r_namespace, Rf_install(fun_name.c_str()));

            if (r_closure == R_UnboundValue) {
                Rf_error(
                    "cannot find function '%s' in namespace of package '%s'",
                    fun_name.c_str(),
                    name_.c_str());
            }

            if (TYPEOF(r_closure) == PROMSXP) {
                r_closure = Rf_eval(r_closure, R_GlobalEnv);
            }

            if (TYPEOF(r_closure) != CLOSXP) {
                Rf_error(
                    "'%s' not bound to a closure in namespace of package '%s'",
                    fun_name.c_str(),
                    name_.c_str());
            }

            function->apply(r_closure, 0, index == size - 1);

            ++index;
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
