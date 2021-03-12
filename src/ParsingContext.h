#ifndef STRICTR_PARSING_CONTEXT_H
#define STRICTR_PARSING_CONTEXT_H

#include "Package.h"

#include <iostream>
#include <string>
#include "Rincludes.h"

namespace parser {
class ParsingContext {
  public:
    explicit ParsingContext(std::istream& input_stream,
                            Package* package)
        : input_stream_(input_stream), package_(package) {
    }

    ~ParsingContext() {
    }

    std::istream& get_input_stream() {
        return input_stream_;
    }

    Package* get_package() {
        return package_;
    }

  private:
    std::istream& input_stream_;
    Package* package_;
};
} // namespace parser

#endif /* STRICTR_PARSING_CONTEXT_H */
