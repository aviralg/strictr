#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Lexer.h"
#include "Parser_autogen.h"
#include "ParsingContext.h"
#include "StrictnessSignatureCache.h"

//#include "parser/Unparser.hpp"
// std::ostream& unparse_(const tastr::ast::Node& node,
//                       std::ostream& os,
//                       bool show_ast,
//                       bool style_output) {
//    tastr::visitor::Unparser unparser(os, show_ast, style_output);
//    unparser.visit(node);
//    return os;
//}
//
// std::ostream& operator<<(std::ostream& os, const tastr::ast::Node& node) {
//    return unparse_(node, os, false, false);
//}
//
// std::string to_string(const tastr::ast::Node& node) {
//    std::string output;
//    unparse_string(node, output, false, false);
//    return output;
//}
//
// void unparse_stdout(const tastr::ast::Node& node,
//                    bool show_ast,
//                    bool style_output) {
//    unparse_(node, std::cout, show_ast, style_output);
//}
//
// void unparse_string(const tastr::ast::Node& node,
//                    std::string& string,
//                    bool show_ast,
//                    bool style_output) {
//    std::ostringstream output_stream;
//    unparse_(node, output_stream, show_ast, style_output);
//    string.append(output_stream.str());
//}
//
// void unparse_file(const tastr::ast::Node& node,
//                  const std::string& filepath,
//                  bool show_ast,
//                  bool style_output) {
//    check_filepath(filepath);
//    std::ofstream output_stream(filepath);
//    unparse_(node, output_stream, show_ast, style_output);
//}

StrictnessSignatureCache parse_(std::istream& input_stream,
                                const std::string& input_stream_name,
                                bool debug_lexer,
                                bool debug_parser) {
    parser::ParsingContext context(input_stream, input_stream_name);
    parser::Lexer lexer(context);
    lexer.set_debug_level(debug_lexer);
    parser::Parser parser(lexer, context);
    parser.set_debug_level(debug_parser);
    parser.parse();
    return context.get_cache();
}

StrictnessSignatureCache parse_stdin(bool debug_lexer, bool debug_parser) {
    std::string input_stream_name("<stdin>");
    return parse_(std::cin, input_stream_name, debug_lexer, debug_parser);
}

StrictnessSignatureCache
parse_string(const std::string& string, bool debug_lexer, bool debug_parser) {
    std::istringstream input_stream(string);
    std::string input_stream_name("<string>");
    return parse_(input_stream, input_stream_name, debug_lexer, debug_parser);
}

StrictnessSignatureCache
parse_file(const std::string& filepath, bool debug_lexer, bool debug_parser) {
    std::ifstream input_stream(filepath);
    std::string input_stream_name(filepath);
    return parse_(input_stream, input_stream_name, debug_lexer, debug_parser);
}
