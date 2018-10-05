#include <sdk.h> // Code::Blocks SDK
#include "cbmarkdownview.h"

#include <wx/sizer.h>
#include <wx/webviewfshandler.h>
#include <wx/fs_mem.h>
#include <wx/filefn.h>

#include "cbstyledtextctrl.h"
#include "textProvider.h"


extern "C"
{
#include "mkdio.h"
}

BEGIN_EVENT_TABLE(cbMarkdownView, wxPanel)

END_EVENT_TABLE()

cbMarkdownView::cbMarkdownView(wxWindow* parent,wxWindowID id)
{
    //ctor
    Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
    // Add temporary file so we can delete it later without error message
    // wxWidgets does not provide any function to check if the memory file exists
    wxMemoryFSHandler::AddFile("cbMarkdownPreviewFile.html","");

    m_sizer = new wxBoxSizer(wxVERTICAL);
    m_curWindowEngine = INVALID; // We have no window set at the moment

    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("markdown"));
	if(!cfg)
        return;

    int renderer = 0;
    cfg->Read("renderEngine",&renderer);

    if(renderer == 0)       SetupWindow(HTML_VIEW);
    else if(renderer == 1)  SetupWindow(WEB_VIEW);
    else                    SetupWindow(INVALID);

    // Set default page
    SetPage(TextProvider::FormatHtmlMessage("No markdown file selected","please select a markdown file. The file ending can be set in Settings->Editor->cbMarkdown"),"");

    SetSizer(m_sizer);
    m_sizer->Fit(this);
    m_sizer->SetSizeHints(this);

}

cbMarkdownView::~cbMarkdownView()
{
    //dtor
}

void cbMarkdownView::OnWebViewNavigate(wxWebViewEvent& event)
{
    // If we open a url from outside the current document load the default browser

    Manager::Get()->GetLogManager()->Log(event.GetURL());
    if(event.GetURL().StartsWith("memory:cbMarkdownPreviewFile.html"))
        event.Skip();
    else
    {
        event.Veto();
        wxLaunchDefaultBrowser(event.GetURL());
    }
}


void cbMarkdownView::SetupWindow(eRenderView window)
{
    if(m_curWindowEngine == window) // nothing to do
        return;

    m_sizer->Clear(true);
    hmtlWindow = nullptr;
    WebViewHmtlWindow = nullptr;

    if(window == HTML_VIEW) // Html View
    {
        hmtlWindow = new cbMarkdownHTMLWindow(this,wxID_ANY, wxPoint(0,0), this->GetSize(),wxHW_SCROLLBAR_AUTO);
        m_sizer->Add(hmtlWindow, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    }
    else if(window == WEB_VIEW) // Web view
    {
        WebViewHmtlWindow = wxWebView::New(this, wxID_ANY, "", wxPoint(0, 0), this->GetSize());
        m_sizer->Add(WebViewHmtlWindow, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

        WebViewHmtlWindow->Bind(wxEVT_WEBVIEW_NAVIGATING, &cbMarkdownView::OnWebViewNavigate, this );
        WebViewHmtlWindow->RegisterHandler(wxSharedPtr<wxWebViewHandler>(new wxWebViewFSHandler("memory")));
    }
    else
    {
        // unknown tech
        return;
    }

    m_curWindowEngine = window;
    m_sizer->Fit(this);
    m_sizer->SetSizeHints(this);

    SetPage(m_curContent, m_curUrl);
}

void cbMarkdownView::SetPage(const wxString& content, const wxString& url)
{
    // Check if content has changed
    // Prevents unneeded scrolling of reselecting the tab
    if(m_curContent == content)
        return;

    Manager::Get()->GetLogManager()->Log(content);

    // Remove old file from memory file system
    wxMemoryFSHandler::RemoveFile("cbMarkdownPreviewFile.html");

    wxMemoryFSHandler::AddFile("cbMarkdownPreviewFile.html", content);

    if(m_curWindowEngine == HTML_VIEW) // Html View
    {
        if(!hmtlWindow)
            SetupWindow(HTML_VIEW);

        hmtlWindow->LoadPage("memory:cbMarkdownPreviewFile.html");
    }
    else if(m_curWindowEngine == WEB_VIEW) // Web view
    {
        if(!WebViewHmtlWindow)
            SetupWindow(WEB_VIEW);

         WebViewHmtlWindow->LoadURL("memory:cbMarkdownPreviewFile.html");
    }
    else
    {
        // unknown tech
        return;
    }

    m_curContent = content;
    m_curUrl = url;
}

void cbMarkdownView::ShowMarkdownOf(EditorBase *eb)
{
    if(eb == NULL)
    {
        SetPage(TextProvider::FormatHtmlMessage("No markdown file selected","please select a markdown file. The file ending can be set in Settings->Editor->cbMarkdown"), "");
        return;
    }
    cbEditor* cbEd = dynamic_cast<cbEditor*>(eb);
    if(cbEd == nullptr)
    {
           SetPage(TextProvider::FormatHtmlMessage("No markdown file selected","please select a markdown file. The file ending can be set in Settings->Editor->cbMarkdown"), "");
        return;
    }
    wxString content = cbEd->GetControl()->GetText();


    MMIOT* mrkdown = mkd_string(content.ToUTF8(), content.size(), MKD_FENCEDCODE | MKD_LATEX );
    mkd_compile(mrkdown, MKD_FENCEDCODE | MKD_LATEX );
    char* html_text;
    int len = mkd_document(mrkdown, &html_text);
    wxString html_wx(html_text, len);
    mkd_cleanup(mrkdown);

    SetPage( wxT("<head>\n<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">\n<style>")+ TextProvider::GetUserCss() + "</style>\n</head>" + html_wx, "");
}

void cbMarkdownView::UpdateMarkdown(EditorBase *eb)
{
    ShowMarkdownOf(eb);
}

wxHtmlOpeningStatus cbMarkdownHTMLWindow::OnOpeningURL(wxHtmlURLType WXUNUSED(type),
                                             const wxString& url,
                                             wxString *redirect) const
{
    Manager::Get()->GetLogManager()->Log(url);
    // Filter URLs that lead to external site and start them in default browser
    if(url.StartsWith("memory:cbMarkdownPreviewFile.html"))
       return wxHTML_OPEN;
    else if(url.StartsWith("http:") ||
            url.StartsWith("https:")||
            url.StartsWith("file:"))
    {
        // redirect to local site
        wxLaunchDefaultBrowser(url);
        *redirect = "memory:cbMarkdownPreviewFile.html";
        return wxHTML_REDIRECT;
    }
    else
        return wxHTML_BLOCK;
}
