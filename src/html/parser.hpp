#ifndef HTML_PARSER_HPP
#define HTML_PARSER_HPP

#include "dom_node.hpp"
#include <string>
#include <memory>

class HtmlParser {
public:
    static std::shared_ptr<DOMNode> parse(const std::string& html);
};

#endif // HTML_PARSER_HPP
