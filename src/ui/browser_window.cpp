#include "browser_window.hpp"
#include "network/http_client.hpp"
#include "html/parser.hpp"
#include "render/renderer.hpp"
#include <iostream>

BrowserWindow::BrowserWindow() : dom_root(nullptr) {
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Ovenpasta");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Navigation bar
    hbox_nav = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox_nav, FALSE, FALSE, 5);

    btn_back = gtk_button_new_with_label("Back");
    gtk_box_pack_start(GTK_BOX(hbox_nav), btn_back, FALSE, FALSE, 0);

    entry_url = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(hbox_nav), entry_url, TRUE, TRUE, 0);

    btn_go = gtk_button_new_with_label("Go");
    gtk_box_pack_start(GTK_BOX(hbox_nav), btn_go, FALSE, FALSE, 0);
    g_signal_connect(btn_go, "clicked", G_CALLBACK(on_go_clicked), this);
    g_signal_connect(entry_url, "activate", G_CALLBACK(on_go_clicked), this);

    // Content area
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);

    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 800, 2000); // Initial size request
    gtk_container_add(GTK_CONTAINER(scrolled_window), drawing_area);
    g_signal_connect(drawing_area, "draw", G_CALLBACK(on_draw), this);
}

BrowserWindow::~BrowserWindow() {
    // GTK handles widget destruction automatically when the top level window is destroyed
}

void BrowserWindow::show() {
    gtk_widget_show_all(window);
}

void BrowserWindow::on_go_clicked(GtkWidget* widget, gpointer data) {
    BrowserWindow* browser = static_cast<BrowserWindow*>(data);
    const char* url_cstr = gtk_entry_get_text(GTK_ENTRY(browser->entry_url));
    std::string url(url_cstr);
    if (!url.empty()) {
        std::cout << "Navigating to: " << url << std::endl;
        browser->load_url(url);
    }
}

gboolean BrowserWindow::on_draw(GtkWidget* widget, cairo_t* cr, gpointer data) {
    BrowserWindow* browser = static_cast<BrowserWindow*>(data);
    
    if (browser->dom_root) {
        Renderer::render(cr, browser->dom_root);
    } else {
        // Fill background with white
        cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
        cairo_paint(cr);

        // Placeholder text
        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
        cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr, 16.0);
        cairo_move_to(cr, 10, 30);
        cairo_show_text(cr, "Welcome to Ovenpasta Browser. Enter a URL to start.");
    }

    return FALSE;
}

void BrowserWindow::load_url(const std::string& url) {
    HttpResponse response = HttpClient::get(url);
    if (response.success) {
        std::cout << "Fetched " << url << " (Status " << response.status_code << ", " << response.body.length() << " bytes)" << std::endl;
        dom_root = HtmlParser::parse(response.body);
    } else {
        std::cout << "Failed to fetch " << url << std::endl;
        dom_root = DOMNode::createElement("html");
        auto body = DOMNode::createElement("body");
        dom_root->children.push_back(body);
        body->children.push_back(DOMNode::createText("Failed to load URL. Note that HTTPS is not supported. Use http://..."));
    }
    
    // Force redraw
    gtk_widget_queue_draw(drawing_area);
}
