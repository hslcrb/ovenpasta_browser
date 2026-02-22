#include "ovenpasta/ovenpasta_view.hpp"
#include "html/dom_node.hpp"
#include "html/parser.hpp"
#include "network/http_client.hpp"
#include "render/renderer.hpp"

#include <iostream>
#include <memory>
#include <gtk/gtk.h>

// Internal C++ implementation class
class OvenpastaViewImpl {
public:
    OvenpastaViewImpl() {
        drawing_area = gtk_drawing_area_new();
        gtk_widget_set_size_request(drawing_area, 800, 2000); // Standard vertical scroll size request
        g_signal_connect(drawing_area, "draw", G_CALLBACK(on_draw_cb), this);
    }

    GtkWidget* get_widget() const {
        return drawing_area;
    }

    void load_url(const std::string& url) {
        HttpResponse response = HttpClient::get(url);
        if (response.success) {
            std::cout << "[Ovenpasta] Fetched " << url << " (Status " << response.status_code << ", " << response.body.length() << " bytes)" << std::endl;
            dom_root = HtmlParser::parse(response.body);
        } else {
            std::cout << "[Ovenpasta] Failed to fetch " << url << std::endl;
            dom_root = DOMNode::createElement("html");
            auto body = DOMNode::createElement("body");
            dom_root->children.push_back(body);
            body->children.push_back(DOMNode::createText("Failed to load URL. Note that HTTPS is not supported. Use http://..."));
        }
        
        gtk_widget_queue_draw(drawing_area);
    }

private:
    GtkWidget* drawing_area;
    std::shared_ptr<DOMNode> dom_root;

    static gboolean on_draw_cb(GtkWidget* widget, cairo_t* cr, gpointer data) {
        OvenpastaViewImpl* view = static_cast<OvenpastaViewImpl*>(data);
        if (view->dom_root) {
            Renderer::render(cr, view->dom_root);
        } else {
            // Default background logic when no URL is loaded
            cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
            cairo_paint(cr);

            cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
            cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
            cairo_set_font_size(cr, 16.0);
            cairo_move_to(cr, 10, 30);
            cairo_show_text(cr, "Welcome to Ovenpasta Embedded Widget!");
        }
        return FALSE;
    }
};

// --- C API Implementation ---

extern "C" {

// We attach the C++ implementation to the GTK widget using g_object_set_data
#define OVENPASTA_IMPL_KEY "ovenpasta_impl_ptr"

static void ovenpasta_view_destroy_cb(GtkWidget* widget, gpointer data) {
    OvenpastaViewImpl* impl = static_cast<OvenpastaViewImpl*>(g_object_get_data(G_OBJECT(widget), OVENPASTA_IMPL_KEY));
    delete impl;
}

GtkWidget* ovenpasta_view_new() {
    OvenpastaViewImpl* impl = new OvenpastaViewImpl();
    GtkWidget* widget = impl->get_widget();
    
    // Manage memory bindings
    g_object_set_data(G_OBJECT(widget), OVENPASTA_IMPL_KEY, impl);
    g_signal_connect(widget, "destroy", G_CALLBACK(ovenpasta_view_destroy_cb), nullptr);
    
    return widget;
}

void ovenpasta_view_load_url(GtkWidget* widget, const char* url) {
    if (!widget || !url) return;
    
    OvenpastaViewImpl* impl = static_cast<OvenpastaViewImpl*>(g_object_get_data(G_OBJECT(widget), OVENPASTA_IMPL_KEY));
    if (impl) {
        impl->load_url(url);
    }
}

} // extern "C"
