#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "html/dom_node.hpp"
#include <gtk/gtk.h>
#include <memory>

class Renderer {
public:
    static void render(cairo_t* cr, std::shared_ptr<DOMNode> root);
private:
    static void render_node(cairo_t* cr, std::shared_ptr<DOMNode> node, double& current_y);
};

#endif // RENDERER_HPP
