//---------------------------------------------------------------------------
#ifndef FGUIHelpH
#define FGUIHelpH
//---------------------------------------------------------------------------
#include "FApplicationGlobals.h"
#include "FGUI.h"
//---------------------------------------------------------------------------

class CGUIHelp : public CGUI
{
public:
	CGUIHelp();
	virtual ~CGUIHelp();
	virtual void Init();
	virtual void Render();
	virtual void Update(float _fDeltaT);
	virtual void CursorDown(int _iX,int _iY,TCursorButton _eButton);
	
protected:
	int m_iCurPage;
	float m_fTime;
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
