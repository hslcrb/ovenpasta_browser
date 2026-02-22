#include "browser_window.hpp"
#include "ovenpasta/ovenpasta_view.hpp"
#include <iostream>

BrowserWindow::BrowserWindow() {
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
    g_signal_connect(btn_back, "clicked", G_CALLBACK(on_back_clicked), this);

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

    // Initialize the embedded Ovenpasta view widget
    drawing_area = ovenpasta_view_new();
    gtk_container_add(GTK_CONTAINER(scrolled_window), drawing_area);
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
        
        // Push to history
        if (browser->history_index < 0 || browser->history[browser->history_index] != url) {
            // Truncate forward history if we diverge
            if (browser->history_index + 1 < browser->history.size()) {
                browser->history.resize(browser->history_index + 1);
            }
            browser->history.push_back(url);
            browser->history_index++;
        }
        
        browser->load_url(url);
    }
}

void BrowserWindow::on_back_clicked(GtkWidget* widget, gpointer data) {
    BrowserWindow* browser = static_cast<BrowserWindow*>(data);
    if (browser->history_index > 0) {
        browser->history_index--;
        std::string prev_url = browser->history[browser->history_index];
        std::cout << "Going Back to: " << prev_url << std::endl;
        
        // Update the UI entry to show where we went back to
        gtk_entry_set_text(GTK_ENTRY(browser->entry_url), prev_url.c_str());
        
        browser->load_url(prev_url);
    } else {
        std::cout << "No history to go back to." << std::endl;
    }
}

void BrowserWindow::load_url(const std::string& url) {
    // Delegate to the Library API
    ovenpasta_view_load_url(drawing_area, url.c_str());
}
