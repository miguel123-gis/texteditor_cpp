#include <FL/Fl_Menu_Bar.H>
#include <FL/fl_ask.H>
#include <FL/filename.H>
#include <FL/fl_string_functions.h>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Editor.H>


// FORWARD DECLARATIONS
void menu_new_callback(Fl_Widget*, void*);
void menu_quit_callback(Fl_Widget*, void*);
void update_title();
void text_changed_callback(int, int n_inserted, int n_deleted, int, const char*, void*);

// GLOBAL VARIABLES
namespace Ted {
    Fl_Double_Window* app_window = NULL;

    Fl_Menu_Bar* app_menu_bar = NULL;

    Fl_Text_Editor* app_editor = NULL;
    Fl_Text_Editor* app_split_editor = NULL;
    Fl_Text_Buffer* app_text_buffer = NULL;
    
    bool text_changed = false;
    char app_filename[FL_PATH_MAX] = "";
}

// WINDOWS
void build_app_window() {
    Ted::app_window = new Fl_Double_Window(640, 480, "FLTK Editor");
}

void build_app_menu_bar() {
    Ted::app_window->begin();
    Ted::app_menu_bar = new Fl_Menu_Bar(0, 0, Ted::app_window->w(), 25);

    Ted::app_menu_bar->add("File/Quit Editor", FL_COMMAND+'q', menu_quit_callback); // Add File>Quit button
    Ted::app_window->callback(menu_quit_callback); // Inherited from Fl_Widget

    int ix = Ted::app_menu_bar->find_index(menu_quit_callback); // Get position of the File>Quit menu
    Ted::app_menu_bar->insert(ix, "New", FL_COMMAND+'n', menu_new_callback); // Insert button for New

    Ted::app_window->end();
}

void build_main_editor() {
    Ted::app_window->begin();
    Ted::app_text_buffer = new Fl_Text_Buffer();
    Ted::app_text_buffer->add_modify_callback(text_changed_callback, NULL);
    Ted::app_editor = new Fl_Text_Editor(
        0,
        Ted::app_menu_bar->h(),
        Ted::app_window->w(),
        Ted::app_window->h() - Ted::app_menu_bar->h()
    );
    Ted::app_editor->buffer(Ted::app_text_buffer);
    Ted::app_editor->textfont(FL_COURIER);
    Ted::app_window->resizable(Ted::app_editor);
    Ted::app_window->end();
}

// OTHER FUNCTIONS
// If content was changed
void set_changed(bool v) {
    if (v != Ted::text_changed) {
        Ted::text_changed = v;
        update_title();
    }
}

// If content was changed
void set_filename(const char* new_filename) {
    if (new_filename) {
        fl_strlcpy(Ted::app_filename, new_filename, FL_PATH_MAX);
    } else {
        Ted::app_filename[0] = 0;
    }
    update_title();
}

void update_title() {
    const char *fname = NULL;
    if (Ted::app_filename[0]) { // Still zero length
        fname = fl_filename_name(Ted::app_filename);
    }

    // If file is already saved
    if (fname) {
        char buf[FL_PATH_MAX + 3];      // For the file extension???
        buf[FL_PATH_MAX + 2] = '\0';    // ensure that the buffer is always null-terminated
        
        if (Ted::text_changed) {
            snprintf(buf, FL_PATH_MAX+2, "%s *", fname);
        } else {
            snprintf(buf, FL_PATH_MAX+2, "%s", fname);
        }

        Ted::app_window->copy_label(buf);
    } else {
        Ted::app_window->label("FLTK Editor");
    }
}

void text_changed_callback(int, int n_inserted, int n_deleted, int, const char*, void*) {
    if (n_inserted || n_deleted) {
        set_changed(true);
    }
}

void menu_new_callback(Fl_Widget*, void*) {
    Ted::app_text_buffer->text("");
    set_changed(false);
}   

void menu_quit_callback(Fl_Widget*, void*) {
    // Fl::hide_all_windows();
    if (Ted::text_changed) {
        int c = fl_choice("Changes in your text have not ben saved.\n"
                          "Do you want to : the editor anyway?",
                          "Quit", "Cancel", NULL);
        if (c == 1) return;
    }
    Fl::hide_all_windows();
}

int main(int argc, char **argv) {
    build_app_window();
    build_app_menu_bar();
    build_main_editor();
    Ted::app_window->show(argc, argv);
    return Fl::run();
}