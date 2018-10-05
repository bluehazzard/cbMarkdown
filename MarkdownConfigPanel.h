#ifndef MARKDOWNCONFIGPANEL_H
#define MARKDOWNCONFIGPANEL_H

//(*Headers(MarkdownConfigPanel)
#include <wx/button.h>
#include <wx/gbsizer.h>
#include <wx/panel.h>
#include <wx/radiobox.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include <configurationpanel.h>

#include "cbmarkdownview.h"

class MarkdownConfigPanel : public cbConfigurationPanel
{
	public:

		MarkdownConfigPanel(wxWindow* parent, cbMarkdownView* view);
		virtual ~MarkdownConfigPanel();

		//(*Declarations(MarkdownConfigPanel)
		wxButton* m_ResetCSS;
		wxRadioBox* m_radRender;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxTextCtrl* m_extensions;
		wxTextCtrl* m_txtCSS;
		//*)

		        /// @return the panel's title.
        virtual wxString GetTitle() const;
        /// @return the panel's bitmap base name. You must supply two bitmaps: \<basename\>.png and \<basename\>-off.png...
        virtual wxString GetBitmapBaseName() const;
        /// Called when the user chooses to apply the configuration.
        virtual void OnApply();
        /// Called when the user chooses to cancel the configuration.
        virtual void OnCancel();

	protected:

		//(*Identifiers(MarkdownConfigPanel)
		static const long ID_RADIOBOX1;
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRL2;
		static const long ID_STATICTEXT2;
		static const long ID_BUTTON1;
		static const long ID_TEXTCTRL1;
		//*)

	private:

	    cbMarkdownView* m_view;

		//(*Handlers(MarkdownConfigPanel)
		void OnResetCSSClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
