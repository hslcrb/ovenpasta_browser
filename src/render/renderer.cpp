#include "renderer.hpp"
#include <iostream>

RenderContext Renderer::render(cairo_t* cr, std::shared_ptr<DOMNode> root) {
    RenderContext ctx;
    if (!root) return ctx;
    
    // Fill background with white
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);

    double current_y = 30.0;
    render_node(cr, root, current_y, ctx);
    return ctx;
}

void Renderer::render_node(cairo_t* cr, std::shared_ptr<DOMNode> node, double& current_y, RenderContext& ctx) {
    if (!node) return;

    if (node->type == NodeType::Text) {
        if (node->text_data.empty()) return;

        if (ctx.in_link) {
            cairo_set_source_rgb(cr, 0.0, 0.0, 1.0); // Blue for links
        } else {
            cairo_set_source_rgb(cr, 0.0, 0.0, 0.0); // Black for normal text
        }
        
        cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr, 14.0);

        cairo_text_extents_t extents;
        cairo_text_extents(cr, node->text_data.c_str(), &extents);

        // Simple text rendering without proper word wrapping for brevity
        cairo_move_to(cr, 10, current_y);
        cairo_show_text(cr, node->text_data.c_str());

        // Underline and save bounding box for links
        if (ctx.in_link && !ctx.current_href.empty()) {
            cairo_move_to(cr, 10, current_y + 2);
            cairo_line_to(cr, 10 + extents.x_advance, current_y + 2);
            cairo_set_line_width(cr, 1.0);
            cairo_stroke(cr);

            RenderedLink link;
            link.url = ctx.current_href;
            link.x = 10;
            // cairo draws from baseline, bounding box starts roughly above baseline
            link.y = current_y - extents.height; 
            link.width = extents.x_advance;
            link.height = extents.height + 4; // Add some padding
            ctx.links.push_back(link);
        }

        current_y += 20.0; // Basic line advance
    } else {
        // Element node
        bool block_level = (node->tag_name == "p" || node->tag_name == "div" || node->tag_name == "h1" || node->tag_name == "h2");
        bool is_header = (node->tag_name == "h1" || node->tag_name == "h2");
        
        bool was_in_link = ctx.in_link;
        std::string prev_href = ctx.current_href;

        if (node->tag_name == "a") {
            ctx.in_link = true;
            if (node->attributes.count("href")) {
                ctx.current_href = node->attributes["href"];
            } else {
                ctx.current_href = "";
            }
        }

        if (is_header) {
             current_y += 10.0; // Margin before
        }

        for (auto child : node->children) {
            render_node(cr, child, current_y, ctx);
        }

        if (block_level) {
            current_y += 10.0; // Margin after
        }

        if (node->tag_name == "a") {
            ctx.in_link = was_in_link;
            ctx.current_href = prev_href;
        }
    }
}
