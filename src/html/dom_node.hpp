#ifndef DOM_NODE_HPP
#define DOM_NODE_HPP

#include <string>
#include <vector>
#include <map>
#include <memory>

enum class NodeType {
    Element,
    Text
};

struct DOMNode {
    NodeType type;
    std::string tag_name;
    std::string text_data;
    std::map<std::string, std::string> attributes;
    std::vector<std::shared_ptr<DOMNode>> children;

    static std::shared_ptr<DOMNode> createElement(const std::string& name);
    static std::shared_ptr<DOMNode> createText(const std::string& text);
};

#endif // DOM_NODE_HPP
