#include "renderer.hpp"
#include <iostream>

void Renderer::render(cairo_t* cr, std::shared_ptr<DOMNode> root) {
    if (!root) return;
    
    // Fill background with white
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);

    double current_y = 30.0;
    render_node(cr, root, current_y);
}

void Renderer::render_node(cairo_t* cr, std::shared_ptr<DOMNode> node, double& current_y) {
    if (!node) return;

    if (node->type == NodeType::Text) {
        if (node->text_data.empty()) return;

        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
        cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr, 14.0);

        // Simple text rendering without proper word wrapping for brevity
        cairo_move_to(cr, 10, current_y);
        cairo_show_text(cr, node->text_data.c_str());
        current_y += 20.0; // Basic line advance
    } else {
        // Element node
        bool block_level = (node->tag_name == "p" || node->tag_name == "div" || node->tag_name == "h1" || node->tag_name == "h2");
        bool is_header = (node->tag_name == "h1" || node->tag_name == "h2");

        if (is_header) {
             current_y += 10.0; // Margin before
        }

        for (auto child : node->children) {
            render_node(cr, child, current_y);
        }

        if (block_level) {
            current_y += 10.0; // Margin after
        }
    }
}
