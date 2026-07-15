#include <FL/Fl_Menu_Bar.H>
#include <FL/fl_ask.H>
#include <FL/filename.H>
#include <FL/fl_string_functions.h>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/platform.H>
#include <errno.h>


// FORWARD DECLARATIONS
void menu_new_callback(Fl_Widget*, void*);
void menu_quit_callback(Fl_Widget*, void*);
void menu_open_callback(Fl_Widget*, void*);
void menu_save_callback(Fl_Widget*, void*);
void menu_save_as_callback(Fl_Widget*, void*);
void load(const char* filename);
void update_title();
void text_changed_callback(int, int n_inserted, int n_deleted, int, const char*, void*);

// GLOBAL VARIABLES
namespace Ted {
    Fl_Double_Window* app_window = NULL;

    Fl_Menu_Bar* app_menu_bar = NULL;

    Fl_Text_Editor* app_editor = NULL;
    Fl_Text_Editor* app_split_editor = NULL;
    Fl_Text_Buffer* app_text_buffer = NULL;

    Fl_Native_File_Chooser* file_chooser = NULL;
    
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

void add_file_support() {
    int ix = Ted::app_menu_bar->find_index(menu_quit_callback); // Finds the "File" button
    Ted::app_menu_bar->insert( // Insert before ix
        ix, 
        "Open",
        FL_COMMAND+'o',
        menu_open_callback,
        NULL,
        FL_MENU_DIVIDER
    );
    Ted::app_menu_bar->insert(ix+1, "Save", FL_COMMAND+'s', menu_save_callback); // Insert after ix
}

void menu_save_callback(Fl_Widget*, void*) {
    if (!Ted::app_filename[0]) { // There's already a filename
        menu_save_as_callback(NULL, NULL);
    } else {
        Ted::app_text_buffer->savefile(Ted::app_filename);
        set_changed(false); // 
    }
}

void menu_save_as_callback(Fl_Widget*, void*) {
    Fl_Native_File_Chooser file_chooser;
    file_chooser.title("Save File As...");
    file_chooser.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);

    // If file is already saved then file_chooser will assume 
    // the file to be saved will have the same extension
    if (Ted::app_filename[0]) { // File is already saved
        char temp_filename[FL_PATH_MAX];
        fl_strlcpy(temp_filename, Ted::app_filename, FL_PATH_MAX); // Copies the current filename to the blank temp_filename without overflowing
        const char* name = fl_filename_name(temp_filename); // Gets filename after copy
        if (name) { 
            file_chooser.preset_file(name); // Auto-fill the fields
            temp_filename[name - temp_filename] = 0; // Remove "somefile.txt" from "/home/jose/somefile.txt"
            file_chooser.directory(temp_filename); // Go to that directory
        }
    }

    // If user already picked a file
    if (file_chooser.show() == 0) { // 0 means user picked a file
        Ted::app_text_buffer->savefile(file_chooser.filename()); // Write buffer to file
        set_filename(file_chooser.filename());
        set_changed(false); // Since file is now "brand new", there are no changes yet
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
        int r = fl_choice("The current file has not been saved.\n"
                          "Would you like to save it now?",
                          "Cancel", "Save", "Don't Save");
        if (r == 0) { // Cancelled
            return;
        } else if (r == 1) { // Save
            menu_save_callback(NULL, NULL);
            return;
        } 
    }
    Fl::hide_all_windows();
}

void menu_open_callback(Fl_Widget*, void*) {
    // Fl::hide_all_windows();
    if (Ted::text_changed) {
        int r = fl_choice("The current file has not been saved.\n"
                          "Would you like to save it now?",
                          "Cancel", "Save", "Don't Save");
        if (r == 2) { // User chose to not save
            return;
        }  
        if (r == 1) { // Save
            menu_save_callback(NULL, NULL);
        } 
    }

    // If user did not cancel operation above
    Ted::file_chooser->title("Open File...");
    Ted::file_chooser->type(Fl_Native_File_Chooser::BROWSE_FILE);
    Fl::hide_all_windows();

    // If file is already saved, fill out the destination folder and file extension
    if (Ted::app_filename[0]) {
        char temp_filename[FL_PATH_MAX];
        fl_strlcpy(temp_filename, Ted::app_filename, FL_PATH_MAX);
        const char* name = fl_filename_name(temp_filename);
        if (name) {
            Ted::file_chooser->preset_file(name);
            temp_filename[name - temp_filename] = 0;
            Ted::file_chooser->directory(temp_filename);
        }
    }

    // If file chooser window is closed, do nothing and keep current file
    if (Ted::file_chooser->show() == 0) {
        load(Ted::file_chooser->filename());
    }
}

void load(const char* filename) {
    if (Ted::app_text_buffer->loadfile(filename)==0) {
        set_filename(filename);
        set_changed(false);
    } else {
        fl_alert("Failed to load file\n%s\n%s", filename, strerror(errno));
    }
}

int main(int argc, char **argv) {
    build_app_window();
    build_app_menu_bar();
    build_main_editor();
    add_file_support();
    Ted::app_window->show(argc, argv);
    return Fl::run();
}