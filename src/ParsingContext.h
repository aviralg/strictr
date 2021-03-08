#ifndef STRICTR_PARSING_CONTEXT_H
#define STRICTR_PARSING_CONTEXT_H

#include "StrictnessSignatureCache.h"

#include <iostream>
#include <string>
#include "Rincludes.h"

namespace parser {
class ParsingContext {
  public:
    explicit ParsingContext(std::istream& input_stream, const std::string& name)
        : input_stream_(input_stream), sig_cache_(name) {
    }

    ~ParsingContext() {
    }

    std::istream& get_input_stream() {
        return input_stream_;
    }

    StrictnessSignatureCache& get_cache() {
        return sig_cache_;
    }

  private:
    std::istream& input_stream_;
    StrictnessSignatureCache sig_cache_;
};
} // namespace parser

#endif /* STRICTR_PARSING_CONTEXT_H */
