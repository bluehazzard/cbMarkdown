#ifndef TEXT_PROVIDER
#define TEXT_PROVIDER

#include <wx/string.h>

class TextProvider
{
    public:

        TextProvider();

        static wxString FormatError(const wxString title, const wxString error);
        static wxString FormatHtmlMessage(const wxString title, const wxString message);

        static wxString GetUserCss();
        static void SetUserCss(const wxString& str);

        static wxString DefaultCss;
        static wxString DefaultMessage;
        static wxString DefaultError;

    private:
        static wxString UserCss;
};


#endif // TEXT_PROVIDER

