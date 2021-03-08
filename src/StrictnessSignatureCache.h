#ifndef STRICTR_STRICTNESS_SIGNATURE_CACHE_H
#define STRICTR_STRICTNESS_SIGNATURE_CACHE_H

#include <string>
#include <unordered_map>
#include "Rincludes.h"
#include "PackageStrictnessSignature.h"

class StrictnessSignatureCache {
  public:
    StrictnessSignatureCache(const std::string& name): name_(name) {
    }

    const PackageStrictnessSignature&
    get_signature(const std::string& package_name,
                  const std::string& version) const {
        std::string key = package_name + "-" + version;
        auto iter = signatures_.find(key);
        if (iter == signatures_.end()) {
            Rf_error("cannot find signature of package '%s-%s' in cache '%s'",
                     package_name.c_str(),
                     version.c_str(),
                     name_.c_str());
        }
        return iter->second;
    }

    void add_signature(PackageStrictnessSignature& pack_sig) {
        const std::string& name = pack_sig.get_name();
        const std::string& version = pack_sig.get_version();
        std::string key = name + "-" + version;
        auto result = signatures_.insert({key, pack_sig});
        if (!result.second) {
            Rf_error("attempt to add signature for already added package "
                     "'%s' in '%s'",
                     name.c_str(),
                     name_.c_str());
        }
    }

    void apply() {
        for (auto iter: signatures_) {
            PackageStrictnessSignature& pack_sig = iter.second;

            const std::string& name = pack_sig.get_name();
            const std::string& version = pack_sig.get_version();
            const std::string loaded_version = get_package_version(name);

            if (version != loaded_version) {
                Rf_warning("Package '%s''s signature version '%s' does not "
                           "match loaded version '%s'",
                           name.c_str(),
                           version.c_str(),
                           loaded_version.c_str());
                continue;
            }

            SEXP r_namespace = get_namespace_(name);

            pack_sig.apply(r_namespace);
        }
    }

    static StrictnessSignatureCache parse(const std::string& dir);

  private:
    std::string name_;
    std::unordered_map<std::string, PackageStrictnessSignature> signatures_;

    std::string get_package_version(const std::string& name) {
        SEXP expr =
            PROTECT(Rf_lang2(Rf_install("print"),
                             Rf_lang2(Rf_install("packageVersion"),
                                      ScalarString(mkChar(name.c_str())))));

        int* r_version = INTEGER(VECTOR_ELT(Rf_eval(expr, R_BaseNamespace), 0));

        UNPROTECT(1);

        return std::to_string(r_version[0]) + "." +
               std::to_string(r_version[1]) + "." +
               std::to_string(r_version[2]);
    }

    SEXP get_namespace_(const std::string& package_name) {
        SEXP r_namespace = Rf_findVarInFrame(R_NamespaceRegistry,
                                             Rf_install(package_name.c_str()));

        if (r_namespace == R_UnboundValue) {
            Rf_error("cannot find namespace for package '%s'",
                     package_name.c_str());
        }

        return r_namespace;
    }
};

StrictnessSignatureCache parse_stdin(bool debug_lexer = false,
                                     bool debug_parser = false);

StrictnessSignatureCache parse_string(const std::string& string,
                                      bool debug_lexer = false,
                                      bool debug_parser = false);

StrictnessSignatureCache parse_file(const std::string& filepath,
                                    bool debug_lexer = false,
                                    bool debug_parser = false);
#endif /* STRICTR_STRICTNESS_SIGNATURE_CACHE_H */
