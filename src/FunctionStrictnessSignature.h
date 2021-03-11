#ifndef STRICTR_FUNCTION_STRICTNESS_SIGNATURE_H
#define STRICTR_FUNCTION_STRICTNESS_SIGNATURE_H

#include <string>
#include <vector>
#include "Rincludes.h"

class FunctionStrictnessSignature {
  public:
    FunctionStrictnessSignature(const std::string& name,
                                const std::vector<int>& signature)
        : name_(name), signature_(signature) {
    }

    FunctionStrictnessSignature(): FunctionStrictnessSignature("unknown", {}) {
    }

    const std::string& get_name() {
        return name_;
    }

    const std::vector<int>& get_signature() const {
        return signature_;
    }

    void apply(SEXP r_closure) {
        SEXP r_formals = FORMALS(r_closure);
        std::vector<SEXP> r_names;

        for (const int parameter: signature_) {
            r_names.push_back(get_parameter_name_(parameter, r_formals));
        }

        SEXP r_body = BODY(r_closure);

        SEXP r_new_body = PROTECT(LCONS(r_body, R_NilValue));

        for (int i = r_names.size() - 1; i >= 0; --i) {
            SEXP r_force_expr = PROTECT(build_force_expr_(r_names[i]));
            r_new_body = PROTECT(LCONS(r_force_expr, r_new_body));
        }

        r_new_body = PROTECT(LCONS(Rf_install("{"), r_new_body));

        SET_BODY(r_closure, r_new_body);

        UNPROTECT(2 + 2 * r_names.size());
    }

  private:
    SEXP get_parameter_name_(int position, SEXP r_formals) {
        int index;
        SEXP r_formal;
        for (index = 1, r_formal = r_formals;
             r_formal != NULL && index <= position;
             r_formal = CDR(r_formal), ++index) {
            if (index == position) {
                SEXP r_name = TAG(r_formal);
                if (TYPEOF(r_name) == SYMSXP) {
                    return r_name;
                } else {
                    Rf_error(
                        "expected parameter name of type symbol at position %d",
                        position);
                }
            }
        }

        Rf_error("argument number %d does not exist", position);
    }

    SEXP build_force_expr_(SEXP r_name) {
        SEXP r_missing = PROTECT(Rf_lang2(Rf_install("missing"), r_name));
        SEXP r_negate = PROTECT(Rf_lang2(Rf_install("!"), r_missing));

        SEXP r_result = Rf_lang3(Rf_install("if"), r_negate, r_name);

        UNPROTECT(2);

        return r_result;
    }

    std::string name_;
    std::vector<int> signature_;
};

#endif /* STRICTR_FUNCTION_STRICTNESS_SIGNATURE_H */
