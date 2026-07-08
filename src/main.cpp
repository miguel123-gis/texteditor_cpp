#include <FL/Fl_Double_Window.H>
#include <FL/Fl.H>

Fl_Double_Window *app_window = NULL;

void build_app_window() {
    app_window = new Fl_Double_Window(640, 480, "FLTK Editor");
}

int main(int argc, char **argv) {
    build_app_window();
    app_window->show(argc, argv);
    return Fl::run();
}