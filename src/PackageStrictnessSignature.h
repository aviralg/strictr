#ifndef STRICTR_PACKAGE_STRICTNESS_SIGNATURE_H
#define STRICTR_PACKAGE_STRICTNESS_SIGNATURE_H

#include "FunctionStrictnessSignature.h"
#include <string>
#include <vector>
#include <unordered_map>
#include "Rincludes.h"

class PackageStrictnessSignature {
  public:
    PackageStrictnessSignature(const std::string& name,
                               const std::string& version)
        : name_(name), version_(version) {
    }

    PackageStrictnessSignature()
        : PackageStrictnessSignature("unknown", "unknown") {
    }

    const std::string& get_name() const {
        return name_;
    }

    const std::string& get_version() const {
        return version_;
    }

    FunctionStrictnessSignature&
    get_signature(const std::string& function_name) {
        auto iter = signatures_.find(function_name);
        if (iter == signatures_.end()) {
            Rf_error(
                "cannot find signature of function '%s' in package '%s-%s'",
                function_name.c_str(),
                name_.c_str(),
                version_.c_str());
        }
        return iter->second;
    }

    void add_signature(FunctionStrictnessSignature& fun_sig) {
        /* TODO; check reinsertion */
        auto result = signatures_.insert({fun_sig.get_name(), fun_sig});
        Rprintf("Added signature for %s", fun_sig.get_name().c_str());
        if (!result.second) {
            Rf_error("attempt to add signature for already added function "
                     "'%s' in '%s-%s'",
                     fun_sig.get_name().c_str(),
                     name_.c_str(),
                     version_.c_str());
        }
    }

    void add_signatures(std::vector<FunctionStrictnessSignature>& fun_sigs) {
        for (auto& iter: fun_sigs) {
            add_signature(iter);
        }
    }

    void apply(SEXP r_namespace) {
        Rprintf("Applying signatures for package '%s-%s'\n",
                name_.c_str(),
                version_.c_str());

        for (auto iter: signatures_) {
            const char* fun_name = iter.first.c_str();
            FunctionStrictnessSignature& fun_sig = iter.second;
            SEXP r_closure =
                Rf_findVarInFrame(r_namespace, Rf_install(fun_name));

            if (r_closure == R_UnboundValue) {
                Rf_error(
                    "cannot find function '%s' in namespace of package '%s-%s'",
                    fun_name,
                    name_.c_str(),
                    version_.c_str());
            }

            if (TYPEOF(r_closure) == PROMSXP) {
                r_closure = Rf_eval(r_closure, R_GlobalEnv);
            }

            if (TYPEOF(r_closure) != CLOSXP) {
                Rf_error("'%s' not bound to function in namespace of package "
                         "'%s-%s'",
                         fun_name,
                         name_.c_str(),
                         version_.c_str());
            }

            fun_sig.apply(r_closure);
        }
    }

  private:
    std::string name_;
    std::string version_;
    std::unordered_map<std::string, FunctionStrictnessSignature> signatures_;
};

#endif /* STRICTR_PACKAGE_STRICTNESS_SIGNATURE_H */
