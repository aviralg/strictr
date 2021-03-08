#ifndef STRICTR_PARSING_CONTEXT_H
#define STRICTR_PARSING_CONTEXT_H

#include "PackageStrictnessSignature.h"

#include <iostream>
#include <string>
#include "Rincludes.h"

namespace parser {
class ParsingContext {
  public:
    explicit ParsingContext(std::istream& input_stream,
                            const std::string& package_name)
        : input_stream_(input_stream), pack_sig_(package_name) {
    }

    ~ParsingContext() {
    }

    std::istream& get_input_stream() {
        return input_stream_;
    }

    PackageStrictnessSignature& get_cache() {
        return pack_sig_;
    }

  private:
    std::istream& input_stream_;
    PackageStrictnessSignature pack_sig_;
};
} // namespace parser

#endif /* STRICTR_PARSING_CONTEXT_H */
