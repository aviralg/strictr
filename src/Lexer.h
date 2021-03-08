#ifndef LEXER_H
#define LEXER_H

#include "Parser_autogen.h"
#include "ParsingContext.h"

/* https://stackoverflow.com/questions/40663527/how-to-inherit-from-yyflexlexer
 */
#if !defined(yyFlexLexerOnce)
#    include <FlexLexer.h>
#endif

/* Tell flex which function to define */
#undef YY_DECL
#define YY_DECL                                                     \
    int parser::Lexer::yylex(parser::Parser::semantic_type* yylval, \
                             parser::Parser::location_type* yylloc)

namespace parser {
class Lexer: public yyFlexLexer {
  public:
    explicit Lexer(ParsingContext& parsing_context);

    virtual int yylex(Parser::semantic_type* yylval,
                      Parser::location_type* yylloc);

    ParsingContext& get_parsing_context() {
        return parsing_context_;
    }

    const ParsingContext& get_parsing_context() const {
        return parsing_context_;
    }

    void set_debug_level(bool debug) {
        yy_flex_debug = debug;
    }

    std::string& get_identifier() {
        return identifier_;
    }

    std::string extract_identifier() {
        std::string result = identifier_;
        identifier_.clear();
        return result;
    }

  private:
    ParsingContext& parsing_context_;
    std::string identifier_;
};
} // namespace parser

#endif /* LEXER_H */
