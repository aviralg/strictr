#ifndef STRICTR_PARSER_H
#define STRICTR_PARSER_H

#include "Package.h"

Package* parse_stdin(Package* package,
                     bool debug_lexer = false,
                     bool debug_parser = false);

Package* parse_string(Package* package,
                      const std::string& string,
                      bool debug_lexer = false,
                      bool debug_parser = false);

Package* parse_file(Package* package,
                    const std::string& filepath,
                    bool debug_lexer = false,
                    bool debug_parser = false);

#endif /* STRICTR_PARSER_H */
