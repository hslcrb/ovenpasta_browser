#ifndef BROWSER_WINDOW_HPP
#define BROWSER_WINDOW_HPP

#include <gtk/gtk.h>
#include <string>
#include <memory>
#include "html/dom_node.hpp"

class BrowserWindow {
public:
    BrowserWindow();
    ~BrowserWindow();

    void show();

private:
    GtkWidget* window;
    GtkWidget* vbox;
    GtkWidget* hbox_nav;
    GtkWidget* entry_url;
    GtkWidget* btn_go;
    GtkWidget* btn_back;
    GtkWidget* scrolled_window;
    GtkWidget* drawing_area;

    static void on_go_clicked(GtkWidget* widget, gpointer data);
    static gboolean on_draw(GtkWidget* widget, cairo_t* cr, gpointer data);
    
    void load_url(const std::string& url);

    std::shared_ptr<DOMNode> dom_root;
};

#endif // BROWSER_WINDOW_HPP
