//---------------------------------------------------------------------------
#include "FGUI.h"
//---------------------------------------------------------------------------
CGUI::CGUI()
{
	m_iScreen  = -1;
	m_bVisible = false;
	m_bEnabled = false;
}
//---------------------------------------------------------------------------
CGUI::~CGUI()
{
}
//---------------------------------------------------------------------------
void CGUI::Init()
{
}
//---------------------------------------------------------------------------
void CGUI::SetScreen(int _iScreen)
{
	if (_iScreen == m_iScreen) return;
	m_iScreen = _iScreen;

	Render();
}
//---------------------------------------------------------------------------
void CGUI::Render()
{
}
//---------------------------------------------------------------------------
void CGUI::CursorDown(int _iX,int _iY,TCursorButton _eButton)
{
}
//---------------------------------------------------------------------------
void CGUI::Update(float _fDeltaT)
{
}
//---------------------------------------------------------------------------
void CGUI::Show()
{
	if (! m_bVisible)
	{
		m_bVisible = true;
		Render();
	}	
};
//---------------------------------------------------------------------------
void CGUI::Enable(bool _bEnable)
{
	m_bEnabled = _bEnable;
}
//---------------------------------------------------------------------------
