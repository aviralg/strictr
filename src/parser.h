#ifndef STRICTR_PARSER_H
#define STRICTR_PARSER_H

#include "PackageStrictnessSignature.h"

PackageStrictnessSignature parse_stdin(const std::string& package_name,
                                       bool debug_lexer = false,
                                       bool debug_parser = false);

PackageStrictnessSignature parse_string(const std::string& package_name,
                                        const std::string& string,
                                        bool debug_lexer = false,
                                        bool debug_parser = false);

PackageStrictnessSignature parse_file(const std::string& package_name,
                                      const std::string& filepath,
                                      bool debug_lexer = false,
                                      bool debug_parser = false);

#endif /* STRICTR_PARSER_H */
