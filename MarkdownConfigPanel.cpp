#include "MarkdownConfigPanel.h"

#include <sdk.h>
#include <wx/file.h>
#include "textProvider.h"

//(*InternalHeaders(MarkdownConfigPanel)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(MarkdownConfigPanel)
const long MarkdownConfigPanel::ID_RADIOBOX1 = wxNewId();
const long MarkdownConfigPanel::ID_STATICTEXT1 = wxNewId();
const long MarkdownConfigPanel::ID_TEXTCTRL2 = wxNewId();
const long MarkdownConfigPanel::ID_STATICTEXT2 = wxNewId();
const long MarkdownConfigPanel::ID_BUTTON1 = wxNewId();
const long MarkdownConfigPanel::ID_TEXTCTRL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(MarkdownConfigPanel,wxPanel)
	//(*EventTable(MarkdownConfigPanel)
	//*)
END_EVENT_TABLE()

MarkdownConfigPanel::MarkdownConfigPanel(wxWindow* parent, cbMarkdownView* view)
{
	//(*Initialize(MarkdownConfigPanel)
	wxBoxSizer* BoxSizer2;
	wxGridBagSizer* GridBagSizer1;
	wxGridBagSizer* GridBagSizer2;
	wxGridBagSizer* GridBagSizer3;

	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	GridBagSizer1 = new wxGridBagSizer(0, 0);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	wxString __wxRadioBoxChoices_1[2] =
	{
		_("Html View (no CSS)"),
		_("Web view")
	};
	m_radRender = new wxRadioBox(this, ID_RADIOBOX1, _("Render technology"), wxDefaultPosition, wxDefaultSize, 2, __wxRadioBoxChoices_1, 1, 0, wxDefaultValidator, _T("ID_RADIOBOX1"));
	BoxSizer2->Add(m_radRender, 0, wxALL|wxALIGN_TOP, 5);
	GridBagSizer2 = new wxGridBagSizer(0, 0);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("File extensions (; seperated list)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	GridBagSizer2->Add(StaticText1, wxGBPosition(0, 0), wxDefaultSpan, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	m_extensions = new wxTextCtrl(this, ID_TEXTCTRL2, _("md;"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	GridBagSizer2->Add(m_extensions, wxGBPosition(1, 0), wxDefaultSpan, wxALL|wxEXPAND, 2);
	GridBagSizer2->AddGrowableCol(0);
	GridBagSizer2->AddGrowableRow(0);
	BoxSizer2->Add(GridBagSizer2, 1, wxALL|wxALIGN_TOP, 5);
	GridBagSizer1->Add(BoxSizer2, wxGBPosition(0, 0), wxDefaultSpan, wxALL|wxEXPAND, 5);
	GridBagSizer3 = new wxGridBagSizer(0, 0);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("CSS used for Web view:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	GridBagSizer3->Add(StaticText2, wxGBPosition(0, 0), wxDefaultSpan, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	m_ResetCSS = new wxButton(this, ID_BUTTON1, _("Reset CSS"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	GridBagSizer3->Add(m_ResetCSS, wxGBPosition(0, 1), wxDefaultSpan, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
	m_txtCSS = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxSize(428,182), wxTE_MULTILINE, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	GridBagSizer3->Add(m_txtCSS, wxGBPosition(1, 0), wxGBSpan(1, 2), wxALL|wxEXPAND, 5);
	GridBagSizer3->AddGrowableCol(0);
	GridBagSizer3->AddGrowableRow(1);
	GridBagSizer1->Add(GridBagSizer3, wxGBPosition(1, 0), wxDefaultSpan, wxEXPAND, 5);
	GridBagSizer1->AddGrowableCol(0);
	GridBagSizer1->AddGrowableRow(1);
	SetSizer(GridBagSizer1);
	GridBagSizer1->Fit(this);
	GridBagSizer1->SetSizeHints(this);

	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MarkdownConfigPanel::OnResetCSSClick);
	//*)

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
