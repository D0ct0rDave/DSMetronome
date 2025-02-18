//---------------------------------------------------------------------------
#ifndef FGUISeqH
#define FGUISeqH
//---------------------------------------------------------------------------
#include "FApplicationGlobals.h"
#include "FGUI.h"
//---------------------------------------------------------------------------
class CGUISeq : public CGUI
{
public:
	CGUISeq();
	virtual void Init();
	virtual void Render();
	virtual void Enable(bool _bEnable);
	virtual void CursorDown(int _iX,int _iY,TCursorButton _eButton);

	virtual void UpdateCursor(int _iNewPos);

private:
	void DrawSeqCursor(bool _bHide = false);
	void DrawSeqMark(int _iX,int _iY,bool _bHide = false);
	void DrawVolumeLine(int _iCh,bool _bInvalidate);

	int	m_iCursor;
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
