#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Lexer.h"
#include "Parser_autogen.h"
#include "ParsingContext.h"
#include "parser.h"

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

PackageStrictnessSignature parse_(const std::string& package_name,
                                  std::istream& input_stream,
                                  bool debug_lexer,
                                  bool debug_parser) {
    parser::ParsingContext context(input_stream, package_name);
    parser::Lexer lexer(context);
    lexer.set_debug_level(debug_lexer);
    parser::Parser parser(lexer, context);
    parser.set_debug_level(debug_parser);
    parser.parse();
    return context.get_cache();
}

PackageStrictnessSignature parse_stdin(const std::string& package_name,
                                       bool debug_lexer,
                                       bool debug_parser) {
    return parse_(package_name, std::cin, debug_lexer, debug_parser);
}

PackageStrictnessSignature parse_string(const std::string& package_name,
                                        const std::string& string,
                                        bool debug_lexer,
                                        bool debug_parser) {
    std::istringstream input_stream(string);
    return parse_(package_name, input_stream, debug_lexer, debug_parser);
}

PackageStrictnessSignature parse_file(const std::string& package_name,
                                      const std::string& filepath,
                                      bool debug_lexer,
                                      bool debug_parser) {
    std::ifstream input_stream(filepath);
    return parse_(package_name, input_stream, debug_lexer, debug_parser);
}
