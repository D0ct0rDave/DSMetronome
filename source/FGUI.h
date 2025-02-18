//---------------------------------------------------------------------------
#ifndef FGUIH
#define FGUIH
//---------------------------------------------------------------------------
#include "FApplicationGlobals.h"
//---------------------------------------------------------------------------

class CGUI
{
public:
	CGUI();
	virtual ~CGUI();
	virtual void Init();

	virtual void Enable(bool _bEnable);

	virtual void Show();
	virtual void Hide() { m_bVisible = false; };
	virtual bool bVisible() { return (m_bVisible); };
	
	virtual void Render();
	virtual void Update(float _fDeltaT);

	virtual void CursorDown(int _iX,int _iY,TCursorButton _eButton);

	virtual void SetScreen(int _iScreen);

public:
	int  m_iScreen;
	bool m_bEnabled;
	bool m_bVisible;
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
