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

            // Parse attributes if there is space after tag name
            if (space_pos != string::npos) {
                string attrs_str = tag_content.substr(space_pos);
                size_t p = 0;
                while (p < attrs_str.length()) {
                    // skip whitespace
                    while (p < attrs_str.length() && std::isspace(attrs_str[p])) p++;
                    if (p >= attrs_str.length()) break;

                    size_t eq_pos = attrs_str.find('=', p);
                    if (eq_pos == string::npos) break; // no more key=value

                    string key = attrs_str.substr(p, eq_pos - p);
                    
                    p = eq_pos + 1;
                    if (p < attrs_str.length() && (attrs_str[p] == '"' || attrs_str[p] == '\'')) {
                        char quote = attrs_str[p];
                        p++;
                        size_t end_quote = attrs_str.find(quote, p);
                        if (end_quote != string::npos) {
                            string value = attrs_str.substr(p, end_quote - p);
                            node->attributes[key] = value;
                            p = end_quote + 1;
                        } else {
                            break; // malformed
                        }
                    } else {
                        // Unquoted attribute value
                        size_t space_end = attrs_str.find_first_of(" \t\n\r/>", p);
                        string value = attrs_str.substr(p, space_end == string::npos ? string::npos : space_end - p);
                        node->attributes[key] = value;
                        p = (space_end == string::npos) ? attrs_str.length() : space_end;
                    }
                }
            }

            bool is_self_closing_tag = tag_content.back() == '/' || is_self_closing(tag_name);

            if (!is_self_closing_tag) {
                stack.push_back(node);
            }
        }
    }

    return root;
}
