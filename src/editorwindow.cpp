#include "globals.h"
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Menu_Bar.H>

class EditorWindow: public Fl_Double_Window 
{
    public:
        EditorWindow(int width, int height, const char* t); // Instantiate class
        ~EditorWindow(); // Destroys this class

        Fl_Window*          replace_dlg;
        Fl_Input*           replace_find;
        Fl_Input*           replace_find;
        Fl_Button*          replace_all;
        Fl_Return_Button*   replace_next;    
        Fl_Button*          replace_cancel;    
        Fl_Text_Editor*     replace_cancel;    
        
        Fl_Text_Editor*     editor;
        char                search[256];

        // Menu bar
        Fl_Menu_Bar *m = new Fl_Menu_Bar(0, 0, 640, 30);
        m->copy(menuitems)

        // Text editor
        width->editor = new Fl_Text_Editor(0, 30, 640, 370);
        width->editor->buffer(textbuf);
        width->editor->textfont(FL_COURIER);

        // Track changes
        textbuf->add_modify_callback(changed_cb, w);
        textbuf->add_modify_callbacks();

        // Replace dialog
        Fl_Window *replace_dlg = new Fl_Window(300, 105, "Replace");
        Fl_Input *replace_find = new Fl_Input(70, 10, 200, 25, "Find:");
        Fl_Input *replace_with = new Fl_Input(70, 40, 200, 25, "Replace:");
        Fl_Button *replace_all = new Fl_Button(10, 70, 90, 25, "Replace All");
        Fl_Button *replace_next = new Fl_Button(105, 70, 120, 25, "Replace Next");
        Fl_Button *replace_cancel = new Fl_Button(230, 70, 60, 25, "Cancel");

    private:
        Fl_Menu_Item menuitems[] = 
        {
            { "&File",              0, 0, 0, FL_SUBMENU },
                { "&New File",        0, (Fl_Callback *)new_cb },
                { "&Open File...",    FL_CTRL + 'o', (Fl_Callback *)open_cb },
                { "&Insert File...",  FL_CTRL + 'i', (Fl_Callback *)insert_cb, 0, FL_MENU_DIVIDER },
                { "&Save File",       FL_CTRL + 's', (Fl_Callback *)save_cb },
                { "Save File &As...", FL_CTRL + FL_SHIFT + 's', (Fl_Callback *)saveas_cb, 0, FL_MENU_DIVIDER },
                { "New &View", FL_ALT + 'v', (Fl_Callback *)view_cb, 0 },
                { "&Close View", FL_CTRL + 'w', (Fl_Callback *)close_cb, 0, FL_MENU_DIVIDER },
                { "E&xit", FL_CTRL + 'q', (Fl_Callback *)quit_cb, 0 },
                { 0 },

            { "&Edit", 0, 0, 0, FL_SUBMENU },
                { "&Undo",       FL_CTRL + 'z', (Fl_Callback *)undo_cb, 0, FL_MENU_DIVIDER },
                { "Cu&t",        FL_CTRL + 'x', (Fl_Callback *)cut_cb },
                { "&Copy",       FL_CTRL + 'c', (Fl_Callback *)copy_cb },
                { "&Paste",      FL_CTRL + 'v', (Fl_Callback *)paste_cb },
                { "&Delete",     0, (Fl_Callback *)delete_cb },
                { 0 },

            { "&Search", 0, 0, 0, FL_SUBMENU },
                { "&Find...",       FL_CTRL + 'f', (Fl_Callback *)find_cb },
                { "F&ind Again",    FL_CTRL + 'g', find2_cb },
                { "&Replace...",    FL_CTRL + 'r', replace_cb },
                { "Re&place Again", FL_CTRL + 't', replace2_cb },
                { 0 },

            { 0 }
        };
};

