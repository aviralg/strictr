#ifndef STRICTR_PACKAGE_STRICTNESS_SIGNATURE_H
#define STRICTR_PACKAGE_STRICTNESS_SIGNATURE_H

#include "FunctionStrictnessSignature.h"
#include <string>
#include <vector>
#include <unordered_map>
#include "Rincludes.h"

class PackageStrictnessSignature {
  public:
    PackageStrictnessSignature(const std::string& name): name_(name) {
    }

    PackageStrictnessSignature(): PackageStrictnessSignature("unknown") {
    }

    const std::string& get_name() const {
        return name_;
    }

    FunctionStrictnessSignature&
    get_signature(const std::string& function_name) {
        auto iter = signatures_.find(function_name);
        if (iter == signatures_.end()) {
            Rf_error("cannot find signature of function '%s' in package '%s'",
                     function_name.c_str(),
                     name_.c_str());
        }
        return iter->second;
    }

    void add_signature(FunctionStrictnessSignature& fun_sig) {
        /* TODO; check reinsertion */
        auto result = signatures_.insert({fun_sig.get_name(), fun_sig});

        if (!result.second) {
            Rf_error("attempt to add signature for already added function "
                     "'%s' in '%s'",
                     fun_sig.get_name().c_str(),
                     name_.c_str());
        }
    }

    void apply() {
        Rprintf("█ Applying signatures for package '%s'\n", name_.c_str());

        SEXP r_namespace = get_namespace_();

        int index = 0;
        int size = signatures_.size();

        for (auto iter: signatures_) {
            FunctionStrictnessSignature& fun_sig = iter.second;
            const std::string& fun_name = fun_sig.get_name();

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

            std::string prefix = "┣━";

            if (index == size - 1) {
                prefix = "┗━";
            }

            Rprintf("%s  %s\n", prefix.c_str(), fun_name.c_str());

            fun_sig.apply(r_closure);

            ++index;
        }
    }

  private:
    std::string name_;
    std::unordered_map<std::string, FunctionStrictnessSignature> signatures_;

    SEXP get_namespace_() {
        SEXP r_namespace =
            Rf_findVarInFrame(R_NamespaceRegistry, Rf_install(name_.c_str()));

        if (r_namespace == R_UnboundValue) {
            Rf_error("cannot find namespace for package '%s'", name_.c_str());
        }

        return r_namespace;
    }
};

#endif /* STRICTR_PACKAGE_STRICTNESS_SIGNATURE_H */
