#ifndef CBMARKDOWNVIEW_H
#define CBMARKDOWNVIEW_H

#include <wx/panel.h>
#include <wx/wxhtml.h>

#include <wx/webview.h>
#include "cbeditor.h"

class EditorBase;

enum eRenderView
{
    INVALID   = 0,
    HTML_VIEW = 1,
    WEB_VIEW
};

class cbMarkdownHTMLWindow : public wxHtmlWindow
{
    public:
        cbMarkdownHTMLWindow(wxWindow *parent , int id, wxPoint pos, wxSize siz, int st) : wxHtmlWindow( parent, id, pos, siz, st ) {};

        virtual wxHtmlOpeningStatus OnOpeningURL(wxHtmlURLType WXUNUSED(type),
                                             const wxString& url,
                                             wxString *WXUNUSED(redirect)) const;

};

class cbMarkdownView : public wxPanel
{
    public:
        cbMarkdownView(wxWindow* parent,wxWindowID id);
        virtual ~cbMarkdownView();

        void ShowMarkdownOf(EditorBase *eb = NULL);
        void UpdateMarkdown(EditorBase *eb);

        void SetupWindow(eRenderView window);
        void SetPage(const wxString& content, const wxString& url);

        void OnWebViewNavigate(wxWebViewEvent& event);

    protected:
        cbMarkdownHTMLWindow*   hmtlWindow;
        wxWebView*      WebViewHmtlWindow;

        wxBoxSizer* m_sizer;

        eRenderView m_curWindowEngine;
        wxString    m_curContent;
        wxString    m_curUrl;


    private:
        DECLARE_EVENT_TABLE()
};

#endif // CBMARKDOWNVIEW_H
