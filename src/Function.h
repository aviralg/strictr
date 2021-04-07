#ifndef STRICTR_FUNCTION_H
#define STRICTR_FUNCTION_H

#include <string>
#include <vector>
#include "Rincludes.h"
#include "Scope.h"
#include "utilities.h"

class Function: public Scope {
  public:
    Function(const std::string& name, const std::vector<int>& signature)
        : Function(name) {
        has_sig_ = true;
        signature_ = signature;
    }

    Function(const std::string& name)
        : Scope(name), has_sig_(false), applied_(false) {
    }

    Function(): Function("unknown") {
    }

    const std::vector<int>& get_signature() const {
        return signature_;
    }

    void set_signature(const std::vector<int>& signature) {
        if (has_sig_) {
            Rf_error("cannot change strictness signature for function '%s'",
                     name_.c_str());
        }
        signature_ = signature;
    }

    void apply(FILE* log_file,
               SEXP r_name,
               SEXP r_namespace,
               SEXP r_expr,
               int level,
               bool last) {
        print_message(log_file, level, last);

        /* TODO: clean up, this is also checked when inserting signature. */
        if (signature_.size() == 0) {
            return;
        }

        SEXP r_body = R_NilValue;

        if (TYPEOF(r_expr) == CLOSXP) {
            /* if this happens, it means the signature is wrong */
            if (!signature_.empty() && Rf_length(FORMALS(r_expr)) < signature_.back()) {
                fprintf(log_file,
                        "incorrect signature for function %s\n",
                        name_.c_str());
                return;
            }

            r_body = PROTECT(apply_signature_(FORMALS(r_expr), BODY(r_expr)));
            SEXP r_srcref = PROTECT(Rf_getAttrib(r_expr, R_SRCREF_SYMBOL));

            // SET_BODY(r_expr, r_body);
            SEXP r_result = PROTECT(
                Rf_eval(Rf_lang3(R_FUNCTION_SYMBOL, FORMALS(r_expr), r_body),
                        r_namespace));

            if (r_srcref != R_NilValue) {
                Rf_setAttrib(r_result, R_SRCREF_SYMBOL, r_srcref);
            }

            int locked = R_BindingIsLocked(r_name, r_namespace);

            if (locked) {
                R_unLockBinding(r_name, r_namespace);
            }

            Rf_setVar(r_name, r_result, r_namespace);

            if (locked) {
                R_LockBinding(r_name, r_namespace);
            }

            UNPROTECT(3);

        } else {
            r_body = apply_signature_(CADR(r_expr), CADDR(r_expr));
            SETCAR(CDDR(r_expr), r_body);
        }

        int size = functions_.size();
        int index = 0;

        applied_ = true;

        // for (auto& iter: functions_) {
        //    Function* inner_function = iter.second;
        //    SEXP r_inner_expr =
        //        find_function_(r_body, inner_function->get_name());
        //
        //    if (r_inner_expr != NULL) {
        //        inner_function->apply(
        //            log_file, r_inner_expr, level + 1, index == size - 1);
        //    }
        //
        //    ++index;
        //}
    }

    void get_status(int level,
                    const std::string& parent_name,
                    std::vector<int>& levels,
                    std::vector<std::string>& names,
                    std::vector<int>& applied) {
        const std::string name = parent_name + name_;
        levels.push_back(level);
        names.push_back(name);
        applied.push_back(applied_);

        for (auto& iter: functions_) {
            iter.second->get_status(
                level + 1, name + "::", levels, names, applied);
        }
    }

  private:
    bool has_sig_;
    std::vector<int> signature_;
    bool applied_;

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

    bool is_vararg_(SEXP r_name) {
        return !strcmp("...", CHAR(PRINTNAME(r_name)));
    }

    SEXP build_force_expr_(SEXP r_name) {
        SEXP r_missing = PROTECT(Rf_lang2(R_MISSING_SYMBOL, r_name));
        SEXP r_negate = PROTECT(Rf_lang2(R_NOT_SYMBOL, r_missing));
        SEXP r_force =
            r_name; // PROTECT(Rf_lang3(R_LEFT_ASSIGN_SYMBOL, r_name, r_name));
        bool vararg = is_vararg_(r_name);

        if (vararg) {
            r_force = PROTECT(Rf_lang2(R_LIST_SYMBOL, r_name));
        }

        SEXP r_result = Rf_lang3(R_IF_SYMBOL, r_negate, r_force);

        UNPROTECT(2 + vararg);

        return r_result;
    }

    SEXP find_function_(SEXP r_body, const std::string& function_name) {
        bool assignment = is_call_(r_body, "<-") || is_call_(r_body, "<<-") ||
                          is_call_(r_body, "=");

        if (assignment) {
            bool is_name = TYPEOF(CADR(r_body)) == SYMSXP;

            bool is_function = is_call_(CADDR(r_body), "function");

            if (is_name && is_function) {
                const char* fun_name = CHAR(PRINTNAME(CADR(r_body)));
                SEXP r_fun_body = CADDR(r_body);
                if (function_name == fun_name) {
                    return r_fun_body;
                }
            }
        }

        if (TYPEOF(r_body) == LANGSXP) {
            for (SEXP r_node = r_body; r_node != R_NilValue;
                 r_node = CDR(r_node)) {
                SEXP r_fun_body = find_function_(CAR(r_node), function_name);

                if (r_fun_body != NULL) {
                    return r_fun_body;
                }
            }
        }

        return NULL;
    }

    bool is_call_(SEXP r_expr, const char* name) {
        if (TYPEOF(r_expr) != LANGSXP)
            return false;
        if (TYPEOF(CAR(r_expr)) != SYMSXP)
            return false;

        const char* actual_name = CHAR(PRINTNAME(CAR(r_expr)));

        return !strcmp(actual_name, name);
    }

    SEXP apply_signature_(SEXP r_formals, SEXP r_body) {
        if (signature_.size() == 0) {
            return r_body;
        }

        std::vector<SEXP> r_names;

        for (const int parameter: signature_) {
            r_names.push_back(get_parameter_name_(parameter, r_formals));
        }

        SEXP r_new_body = PROTECT(LCONS(r_body, R_NilValue));

        for (int i = r_names.size() - 1; i >= 0; --i) {
            SEXP r_name = r_names[i];
            SEXP r_force_expr = PROTECT(build_force_expr_(r_name));
            r_new_body = PROTECT(LCONS(r_force_expr, r_new_body));
        }

        r_new_body = PROTECT(LCONS(R_LEFT_BRACE_SYMBOL, r_new_body));

        UNPROTECT(2 + 2 * r_names.size());

        return r_new_body;
    }

    void print_message(FILE* log_file, int level, int last) {
        std::string connectors;
        std::string space(2, ' ');

        for (int i = 0; i < level; ++i) {
            connectors.append("┃");
            connectors.append(space);
        }

        std::string prefix = last ? "┗━━█" : "┣━━█";

        fprintf(log_file,
                "%s%s %s\n",
                connectors.c_str(),
                prefix.c_str(),
                name_.c_str());
    }
};

#endif /* STRICTR_FUNCTION_H */
