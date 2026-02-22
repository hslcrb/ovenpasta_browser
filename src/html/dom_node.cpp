#include "dom_node.hpp"

std::shared_ptr<DOMNode> DOMNode::createElement(const std::string& name) {
    auto node = std::make_shared<DOMNode>();
    node->type = NodeType::Element;
    node->tag_name = name;
    return node;
}

std::shared_ptr<DOMNode> DOMNode::createText(const std::string& text) {
    auto node = std::make_shared<DOMNode>();
    node->type = NodeType::Text;
    node->text_data = text;
    return node;
}
