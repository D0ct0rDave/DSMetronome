//---------------------------------------------------------------------------
#ifndef FGUIMetroH
#define FGUIMetroH
//---------------------------------------------------------------------------
#include "FApplicationGlobals.h"
#include "FGUI.h"
//---------------------------------------------------------------------------
class CGUIMetro : public CGUI
{
public:
	virtual void Init();
	virtual void SetScreen(int _iScreen);
	virtual void Render();
	virtual void Enable(bool _bEnable);
	virtual void CursorDown(int _iX,int _iY,TCursorButton _eButton);

	void SetBPMs(int _iBPMs);
	void SetTempoNum(int _iTempoNum);

private:
	void SetupBPMSprites();
	void SetupTempoNumSprite();
	void SetupOnOffSprite();
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
