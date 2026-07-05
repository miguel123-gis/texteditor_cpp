#include <globals.h>
#include <FL/Fl_Double_Window.H>

class EditorWindow: public FL_Double_Window 
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
};

Fl_Menu_Item[] = 
{
    { "&File", 0, 0, 0, FL_SUBMENU },
        { "&New File", 0, (Fl_Callback *)new_cb },
        { "&Open File", FL_CTRL + 'o', (Fl_Callback *)new_cb },
        { "&Insert File...", FL_CTRL + 'i', (Fl_Callback *)insert_cb, 0, FL_MENU_DIVIDER },
}