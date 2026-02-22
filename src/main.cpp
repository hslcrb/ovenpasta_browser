#include <gtk/gtk.h>
#include "ui/browser_window.hpp"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    BrowserWindow browser;
    browser.show();

    gtk_main();

    return 0;
}
