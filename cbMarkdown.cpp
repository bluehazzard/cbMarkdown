#include <sdk.h> // Code::Blocks SDK
#include <configurationpanel.h>
#include "cbMarkdown.h"
#include "MarkdownConfigPanel.h"
#include "textProvider.h"
#include <wx/tokenzr.h>

// Register the plugin with Code::Blocks.
// We are using an anonymous namespace so we don't litter the global one.
namespace
{
    PluginRegistrant<cbMarkdown> reg(_T("cbMarkdown"));

    const long idViewMiniDocPanel = wxNewId();
}


// events handling
BEGIN_EVENT_TABLE(cbMarkdown, cbPlugin)
    // add any events you want to handle here
    EVT_MENU(idViewMiniDocPanel,      cbMarkdown::OnViewMiniDocPanel)
    EVT_UPDATE_UI(idViewMiniDocPanel, cbMarkdown::OnUpdateViewMenu)
END_EVENT_TABLE()

// constructor
cbMarkdown::cbMarkdown()
{
    // Make sure our resources are available.
    // In the generated boilerplate code we have no resources but when
    // we add some, it will be nice that this code is in place already ;)
    if(!Manager::LoadResource(_T("cbMarkdown.zip")))
    {
        NotifyMissingFile(_T("cbMarkdown.zip"));
    }
}

// destructor
cbMarkdown::~cbMarkdown()
{
}

void cbMarkdown::OnAttach()
{
    // do whatever initialization you need for your plugin
    // NOTE: after this function, the inherited member variable
    // m_IsAttached will be TRUE...
    // You should check for it in other functions, because if it
    // is FALSE, it means that the application did *not* "load"
    // (see: does not need) this plugin...

    m_view = new cbMarkdownView(Manager::Get()->GetAppWindow(), wxID_ANY);
    if(!m_view)
    {
        Manager::Get()->GetLogManager()->Log(_T("Could not create Markdown view!"));
        return;
    }

    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("markdown"));

    CodeBlocksDockEvent dockevt(cbEVT_ADD_DOCK_WINDOW);
    dockevt.name = _T("Markdown");
    dockevt.title = _("Markdown");
    dockevt.pWindow = m_view;
    dockevt.minimumSize.Set(50, 50);
    dockevt.desiredSize.Set(150, 100);
    dockevt.floatingSize.Set(100, 150);
    dockevt.dockSide = CodeBlocksDockEvent::dsRight;
    dockevt.stretch = true;
    Manager::Get()->ProcessEvent(dockevt);

    Manager::Get()->RegisterEventSink(cbEVT_EDITOR_ACTIVATED,   new cbEventFunctor<cbMarkdown, CodeBlocksEvent>(this, &cbMarkdown::OnEditorActivated));
    Manager::Get()->RegisterEventSink(cbEVT_EDITOR_SAVE,   new cbEventFunctor<cbMarkdown, CodeBlocksEvent>(this, &cbMarkdown::OnEditorSaved));
    Manager::Get()->RegisterEventSink(cbEVT_EDITOR_SWITCHED,   new cbEventFunctor<cbMarkdown, CodeBlocksEvent>(this, &cbMarkdown::OnEditorSwitched));

    // Read CSS file
    wxString css_path = ConfigManager::GetFolder(sdDataUser) + wxT("/markdown.css");

    if(!wxFileExists(css_path))
    {
        Manager::Get()->GetLogManager()->Log(wxT("Could not load markdown css file"));
        TextProvider::SetUserCss(TextProvider::DefaultCss);
        wxFileOutputStream output( css_path );
        wxTextOutputStream fout( output );

        if(output.IsOk())
        {
            Manager::Get()->GetLogManager()->Log(wxT("Markdown default css file created"));
            fout << TextProvider::DefaultCss;
            output.Close();
        }
        else
            Manager::Get()->GetLogManager()->LogError(wxT("Could not create markdown css file"));
    }
    else
    {
        wxFileInputStream input(css_path);
        wxTextInputStream text(input, wxT(" \t\n"), wxConvUTF8 );
        wxString css_file;
        while(input.IsOk() && !input.Eof())
        {
            css_file += text.ReadLine() +wxT("\n");
        };
        TextProvider::SetUserCss(css_file);
    }
}

void cbMarkdown::OnRelease(bool appShutDown)
{
    // do de-initialization for your plugin
    // if appShutDown is true, the plugin is unloaded because Code::Blocks is being shut down,
    // which means you must not use any of the SDK Managers
    // NOTE: after this function, the inherited member variable
    // m_IsAttached will be FALSE...

        CodeBlocksDockEvent docevt(cbEVT_REMOVE_DOCK_WINDOW);
        docevt.pWindow = m_view;
        Manager::Get()->ProcessEvent(docevt);
        // and destroy the panel
        m_view->Destroy();
}

void cbMarkdown::BuildMenu(wxMenuBar* menuBar)
{
        // insert entry in the View menu
    int viewPos = menuBar->FindMenu(_("&View"));
    if (viewPos != wxNOT_FOUND)
    {
        m_pViewMenu = menuBar->GetMenu(viewPos);
        wxMenuItemList& items = m_pViewMenu->GetMenuItems();
        // find the first separator and insert before it
        for (size_t i = 0; i < items.GetCount(); ++i)
        {
            if (items[i]->IsSeparator())
            {
                m_pViewMenu->InsertCheckItem(i, idViewMiniDocPanel, _("&Markdown view"), _("Toggle displaying the Markdown viewer panel"));
                return;
            }
        }
        // not found so just append
        m_pViewMenu->AppendCheckItem(idViewMiniDocPanel, _("&Markdown view"), _("Toggle displaying the Markdown viewer panel"));
    }
}

void cbMarkdown::OnViewMiniDocPanel(wxCommandEvent& event)
{
    CodeBlocksDockEvent evt(event.IsChecked() ? cbEVT_SHOW_DOCK_WINDOW : cbEVT_HIDE_DOCK_WINDOW);
    evt.pWindow = m_view;
    Manager::Get()->ProcessEvent(evt);
}
void cbMarkdown::OnUpdateViewMenu(wxUpdateUIEvent &event)
{
    if (m_pViewMenu)
    {
        bool isVis = IsWindowReallyShown((wxWindow*)m_view);
        m_pViewMenu->Check(idViewMiniDocPanel, isVis);
    }
    // must do...
    event.Skip();
}
void cbMarkdown::OnEditorActivated(CodeBlocksEvent& event)
{
    if (m_view && IsAttached())
    {
        ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("markdown"));
        if(!cfg)
        {
            Manager::Get()->GetLogManager()->LogError(wxT("Could not get cfg manager for markdown"));
            return;
        }

        wxString extension = wxT("md");
        cfg->Read("extension",&extension);

        wxStringTokenizer tkz(extension,wxT(";"));

        EditorBase* edit = event.GetEditor();
        while(tkz.HasMoreTokens())
        {
            if(edit->GetTitle().After('.') == tkz.GetNextToken())
            {
                m_view->ShowMarkdownOf(edit);
                m_curEd = edit;
                Manager::Get()->GetLogManager()->DebugLog(wxT("Open editor for markdown: ") + edit->GetTitle());
                break;
            }
        }
    }
}

void cbMarkdown::OnEditorSaved(CodeBlocksEvent& event)
{
    if (m_view && IsAttached())
    {
        EditorBase* edit = event.GetEditor();
        if(edit == m_curEd)
        {
            m_view->UpdateMarkdown(edit);
            Manager::Get()->GetLogManager()->DebugLog(wxT("Update after saving: ") + edit->GetTitle());
        }
    }
}

void cbMarkdown::OnEditorSwitched(CodeBlocksEvent& event)
{
    if (m_view && IsAttached())
    {
    }
}

void cbMarkdown::OnEditorClose(CodeBlocksEvent& event)
{
    if (m_view && IsAttached())
    {
        EditorBase* edit = event.GetEditor();
        if(edit == m_curEd)
        {
            m_view->UpdateMarkdown(NULL);
            m_curEd = NULL;
            Manager::Get()->GetLogManager()->DebugLog(wxT("Update after saving: ") + edit->GetTitle());
        }
    }
}

cbConfigurationPanel* cbMarkdown::GetConfigurationPanel(wxWindow* parent)
{
    return new MarkdownConfigPanel(parent, m_view);
}

