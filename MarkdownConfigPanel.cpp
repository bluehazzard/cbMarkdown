#include "MarkdownConfigPanel.h"

#include <sdk.h>
#include <wx/file.h>
#include "textProvider.h"

//(*InternalHeaders(MarkdownConfigPanel)




//(*IdInit(MarkdownConfigPanel)








BEGIN_EVENT_TABLE(MarkdownConfigPanel,wxPanel)
	//(*EventTable(MarkdownConfigPanel)
	//*)
END_EVENT_TABLE()

MarkdownConfigPanel::MarkdownConfigPanel(wxWindow* parent, cbMarkdownView* view)
{
	//(*Initialize(MarkdownConfigPanel)











































	ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("markdown"));
	if(!cfg)
        return;

    int renderer = 0;
    cfg->Read("renderEngine",&renderer);
    m_radRender->SetSelection(renderer);
    m_view = view;

    wxString extension;
    cfg->Read("extension",&extension);
    m_extensions->SetValue(extension);

        // Read CSS file
    wxString css_path = ConfigManager::GetFolder(sdDataUser) + wxT("/markdown.css");

    if(!wxFileExists(css_path))
    {
        // File does not exists so we use the default css
        m_txtCSS->SetValue(TextProvider::DefaultCss);
    }
    else
    {
        wxFileInputStream input(css_path);
        wxTextInputStream text(input, wxT(" \t\n"), wxConvUTF8 );
        wxString css_file;
        while(input.IsOk() && !input.Eof())
        {
            css_file += text.ReadLine() + wxT("\n");
        };
        m_txtCSS->SetValue(css_file);
        TextProvider::SetUserCss(css_file);
    }

}

MarkdownConfigPanel::~MarkdownConfigPanel()
{
	//(*Destroy(MarkdownConfigPanel)
	//*)
}

void MarkdownConfigPanel::OnApply()
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("markdown"));
	if(!cfg)
        return;

    cfg->Write("renderEngine",m_radRender->GetSelection());
    cfg->Write("extension",m_extensions->GetValue());

    if(m_view != nullptr)
    {
        switch(m_radRender->GetSelection())
        {
            case 0: m_view->SetupWindow(HTML_VIEW); break;
            case 1: m_view->SetupWindow(WEB_VIEW);  break;
        }
    }

    wxString css_path = ConfigManager::GetFolder(sdDataUser) + wxT("/markdown.css");
    wxFile output(css_path, wxFile::write);
    /*if(wxFile::Exists(css_path));
        output.open*/

    if(output.IsOpened())
    {
        output.Write(m_txtCSS->GetValue(), wxConvUTF8);
        TextProvider::SetUserCss(m_txtCSS->GetValue());
        output.Close();
    }
    else
    {
        Manager::Get()->GetLogManager()->LogError(wxT("Can not create css file: ") + css_path);
    }

}

void MarkdownConfigPanel::OnCancel()
{
}

wxString MarkdownConfigPanel::GetTitle() const
{
    return _("cbMarkdown");
}

wxString MarkdownConfigPanel::GetBitmapBaseName() const
{
    return _T("cbMarkdown");
}

void MarkdownConfigPanel::OnResetCSSClick(wxCommandEvent& event)
{
    m_txtCSS->SetValue(TextProvider::DefaultCss);
}