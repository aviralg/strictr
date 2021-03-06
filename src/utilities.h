#ifndef STRICTR_UTILITIES_H
#define STRICTR_UTILITIES_H

#include <string>
#include <vector>
#include <string>
#include "Rincludes.h"

extern SEXP R_MISSING_SYMBOL;
extern SEXP R_NOT_SYMBOL;
extern SEXP R_LEFT_ASSIGN_SYMBOL;
extern SEXP R_LIST_SYMBOL;
extern SEXP R_IF_SYMBOL;
extern SEXP R_LEFT_BRACE_SYMBOL;
extern SEXP R_FUNCTION_SYMBOL;
extern SEXP R_SRCREF_SYMBOL;

std::string read_file(const std::string& filepath);

bool file_exists(const std::string& filepath);

extern const std::string STRICTR_NA_STRING;

std::string get_type_as_string(SEXP r_object);

SEXP integer_vector_wrap(const std::vector<int>& vector);

SEXP real_vector_wrap(const std::vector<double>& vector);

SEXP character_vector_wrap(const std::vector<std::string>& vector);

SEXP logical_vector_wrap(const std::vector<int>& vector);

void set_class(SEXP r_object, const std::string& class_name);

SEXP create_data_frame(const std::vector<std::string>& names,
                       const std::vector<SEXP>& columns);

#endif /* STRICTR_UTILITIES_H */
