#include "parser.hpp"
#include <cctype>
#include <iostream>

using std::string;
using std::shared_ptr;

static bool is_self_closing(const string& tag) {
    return tag == "meta" || tag == "link" || tag == "img" || tag == "br" || tag == "hr" || tag == "input";
}

shared_ptr<DOMNode> HtmlParser::parse(const string& html) {
    auto root = DOMNode::createElement("html");
    std::vector<shared_ptr<DOMNode>> stack;
    stack.push_back(root);

    size_t pos = 0;
    while (pos < html.length()) {
        size_t next_bracket = html.find('<', pos);
        
        // Text node
        if (next_bracket != pos) {
            string text = html.substr(pos, next_bracket == string::npos ? string::npos : next_bracket - pos);
            bool only_whitespace = true;
            for (char c : text) {
                if (!std::isspace(c)) {
                    only_whitespace = false;
                    break;
                }
            }
            if (!only_whitespace) {
                stack.back()->children.push_back(DOMNode::createText(text));
            }
            if (next_bracket == string::npos) break;
            pos = next_bracket;
        }

        // Tag
        size_t end_bracket = html.find('>', pos);
        if (end_bracket == string::npos) break;

        string tag_content = html.substr(pos + 1, end_bracket - pos - 1);
        pos = end_bracket + 1;

        if (tag_content.empty()) continue;

        if (tag_content[0] == '/') {
            // Closing tag
            string tag_name = tag_content.substr(1);
            // simple pop
            if (stack.size() > 1) { // Never pop the root manually unless needed
                stack.pop_back();
            }
        } else if (tag_content[0] == '!') {
            // Comment or DOCTYPE, ignore
        } else {
            // Opening tag
            size_t space_pos = tag_content.find_first_of(" \t\n\r");
            string tag_name = tag_content.substr(0, space_pos);
            
            auto node = DOMNode::createElement(tag_name);
            stack.back()->children.push_back(node);

            // Rough attributes parsing (skipped for simplicity in this basic version)

            bool is_self_closing_tag = tag_content.back() == '/' || is_self_closing(tag_name);

            if (!is_self_closing_tag) {
                stack.push_back(node);
            }
        }
    }

    return root;
}
