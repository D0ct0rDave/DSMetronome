//---------------------------------------------------------------------------
#include "FGUISeq.h"
#include <PA9.h>
#include "FApplicationGlobals.h"
#include "all_gfx.h"
#include "FSoundSys.h"
//---------------------------------------------------------------------------
#define SEQMARK_COLOR				FONT_COLOR
#define SEQGRID_COLOR				PA_RGB(16,9,0)
#define SEQGRID_INACTIVECOLOR		PA_RGB(8,4,0)
#define SEQVOLLINE_COLOR			FONT_COLOR

const int SEQ_CELLW				= 9;
const int SEQ_CELLH				= 13;

const int SEQ_XOFS				= 30;
const int SEQ_YOFS				= 58;
const int SEQ_INSTRUM_WIDTH		= 48;
const int SEQ_INSTRUM_HEIGHT	= MAX_INSTRUMENTS*SEQ_CELLH;

const int SEQ_CELLS_XOFS		= (SEQ_XOFS + SEQ_INSTRUM_WIDTH);
const int SEQ_CELLS_YOFS		= SEQ_YOFS;

const int SEQ_VOL_XOFS			= SEQ_XOFS + 2;
const int SEQ_VOL_YOFS			= SEQ_YOFS + SEQ_CELLH - 2;
const int SEQ_MAX_VOLWIDTH		= SEQ_INSTRUM_WIDTH-4;
//---------------------------------------------------------------------------
CGUISeq::CGUISeq()
{
	m_iCursor = 0;
}
//---------------------------------------------------------------------------
void CGUISeq::Init()
{
}
//---------------------------------------------------------------------------
void CGUISeq::UpdateCursor(int _iNewPos)
{
	if (_iNewPos == m_iCursor) return;
	
	if (bVisible())
	{
		DrawSeqCursor(true);
		m_iCursor = _iNewPos;
		if (bVisible()) DrawSeqCursor();
	}

	m_iCursor = _iNewPos;
}
//---------------------------------------------------------------------------
void CGUISeq::Enable(bool _bEnable)
{
	CGUI::Enable(_bEnable);

	if (_bEnable == false)
		DrawSeqCursor(true);
}
//---------------------------------------------------------------------------
void CGUISeq::DrawVolumeLine(int _iCh,bool _bInvalidate)
{
	int iYOfs = SEQ_VOL_YOFS + _iCh * SEQ_CELLH;
	int iWidth = (globals.m_oInstTab[_iCh].m_ucVol * SEQ_MAX_VOLWIDTH) / MAX_INST_VOL;

	// Clear the previous volume line
	if (_bInvalidate)
		PA_Draw16bitLine(m_iScreen,	SEQ_VOL_XOFS,iYOfs, SEQ_VOL_XOFS + SEQ_MAX_VOLWIDTH,iYOfs,0);

	PA_Draw16bitLine(m_iScreen,	SEQ_VOL_XOFS,iYOfs, SEQ_VOL_XOFS + iWidth,iYOfs,SEQVOLLINE_COLOR);
}
//---------------------------------------------------------------------------
void CGUISeq::DrawSeqCursor(bool _bHide)
{
	u16 _uColor = _bHide?0:SEQMARK_COLOR;

	int _iX = SEQ_CELLS_XOFS + m_iCursor * SEQ_CELLW + 5;
	int _iY = SEQ_CELLS_YOFS - 3;

	PA_Put16bitPixel(m_iScreen,_iX-3, _iY-3, _uColor);
	PA_Put16bitPixel(m_iScreen,_iX-2, _iY-2, _uColor);
	PA_Put16bitPixel(m_iScreen,_iX-1, _iY-1, _uColor);
	PA_Put16bitPixel(m_iScreen,_iX-0, _iY-0, _uColor);	
	PA_Put16bitPixel(m_iScreen,_iX+1, _iY-1, _uColor);	
	PA_Put16bitPixel(m_iScreen,_iX+2, _iY-2, _uColor);
	PA_Put16bitPixel(m_iScreen,_iX+3, _iY-3, _uColor);
}
//---------------------------------------------------------------------------
void CGUISeq::DrawSeqMark(int _iX,int _iY,bool _bHide)
{
	u16 _uColor = _bHide?0:SEQMARK_COLOR;

	_iX = SEQ_CELLS_XOFS + _iX * SEQ_CELLW + 5;
	_iY = SEQ_CELLS_YOFS + _iY * SEQ_CELLH + 7;

	PA_Put16bitPixel(m_iScreen,_iX-1, _iY-1, _uColor);
	PA_Put16bitPixel(m_iScreen,_iX-0, _iY-0, _uColor);
	PA_Put16bitPixel(m_iScreen,_iX+1, _iY+1, _uColor);
	PA_Put16bitPixel(m_iScreen,_iX+1, _iY-1, _uColor);
	PA_Put16bitPixel(m_iScreen,_iX-1, _iY+1, _uColor);
}
//---------------------------------------------------------------------------
inline void DrawVerticalLine(int _iScreen,int _iLine,u16 _uColor)
{	
	int	iXOfs;

	if (_iLine == -1)
		iXOfs = SEQ_XOFS;
	else
		iXOfs = SEQ_CELLS_XOFS + _iLine*SEQ_CELLW;
		
	PA_Draw16bitLine(_iScreen,iXOfs,SEQ_CELLS_YOFS,iXOfs,SEQ_YOFS+SEQ_INSTRUM_HEIGHT,_uColor);	
}
//---------------------------------------------------------------------------
inline void DrawHorizontalLine(int _iScreen,int _iLine,int _iStartCol,int _iEndCol,u16 _uColor)
{	
	int iYOfs = SEQ_YOFS + _iLine*SEQ_CELLH;
	
	if (_iStartCol == -1)
		PA_Draw16bitLine(_iScreen,SEQ_XOFS                                           , iYOfs,
								  SEQ_XOFS + SEQ_INSTRUM_WIDTH +  _iEndCol *SEQ_CELLW, iYOfs,
								  _uColor);	
	else
		PA_Draw16bitLine(_iScreen,SEQ_CELLS_XOFS + _iStartCol*SEQ_CELLW, iYOfs,
								  SEQ_CELLS_XOFS + _iEndCol *SEQ_CELLW, iYOfs,
								  _uColor);
}
//---------------------------------------------------------------------------
void CGUISeq::Render()
{
	// Do not render if not visible
	if (! bVisible()) return;
	if (m_iScreen == -1) return;

	PA_ClearTextBg(m_iScreen);

	PA_Clear16bitBg(m_iScreen);
	PA_Init16bitBg(m_iScreen, 3);
	PA_Load16bitBitmap(m_iScreen,screen0_Bitmap);

	int i;
	int iMaxTics = globals.m_iTempoNum * TICS_PER_BEAT;

	// Draw inactive lines (if there are any)
	if (iMaxTics < MAX_TICS)
	{
		for (i=0;i<MAX_INSTRUMENTS+1;i++)
			DrawHorizontalLine(m_iScreen,i,iMaxTics,MAX_TICS,SEQGRID_INACTIVECOLOR);

		for (i=iMaxTics+1;i<MAX_TICS+1;i++)
			DrawVerticalLine(m_iScreen,i,SEQGRID_INACTIVECOLOR);
	}

	// Draw first line
	DrawVerticalLine(m_iScreen,-1,SEQGRID_COLOR);

	// Draw active lines
	for (i=0;i<MAX_INSTRUMENTS+1;i++)
		DrawHorizontalLine(m_iScreen,i,-1,iMaxTics,SEQGRID_COLOR);

	for (i=0;i<iMaxTics+1;i++)
		DrawVerticalLine(m_iScreen,i,SEQGRID_COLOR);

	// Draw marks belonging to the "active" range
	int j;
	for (i=0;i<MAX_INSTRUMENTS;i++)
		for (j=0;j<iMaxTics;j++)
			if (globals.m_ucPatternGrid[i][j] == '1')
				DrawSeqMark(j,i);

	DrawSeqCursor(m_iCursor);
	
	// Draw volume lines
	for (i=0;i<MAX_INSTRUMENTS;i++)
		DrawVolumeLine(i,false);
}
//---------------------------------------------------------------------------
void CGUISeq::CursorDown(int _iX,int _iY,TCursorButton _eButton)
{
	// Theoricatelly we cannot receive inputs if the GUI is not visible!
	if (! bVisible()) return;
	
	// Check if point is inside bounding box
	int iMaxTics = globals.m_iTempoNum * TICS_PER_BEAT;
	
	if (
		(_iX>=SEQ_CELLS_XOFS) && (_iX < (SEQ_CELLS_XOFS+SEQ_CELLW*iMaxTics))
	  &&(_iY>=SEQ_CELLS_YOFS) && (_iY < (SEQ_CELLS_YOFS+SEQ_CELLH*MAX_INSTRUMENTS))
	   )
	{
		_iX -= SEQ_CELLS_XOFS;
		_iY -= SEQ_CELLS_YOFS;	

		_iX /= SEQ_CELLW;
		_iY /= SEQ_CELLH;

		if (globals.m_ucPatternGrid[_iY][_iX] == '1')
		{
			// Erase the mark
			DrawSeqMark(_iX,_iY,true);
			globals.m_ucPatternGrid[_iY][_iX] = '0';
		}
		else
		{
			// Erase the mark
			DrawSeqMark(_iX,_iY);
			globals.m_ucPatternGrid[_iY][_iX] = '1';
		}

		CSoundSys::Play(FEEDBACKSOUND);
	}	
else if (
		(_iX>=SEQ_XOFS+2) && (_iX < (SEQ_CELLS_XOFS-1))
	  &&(_iY>=SEQ_YOFS) && (_iY < (SEQ_CELLS_YOFS+SEQ_CELLH*MAX_INSTRUMENTS))
	   )
	{
		// get the pointed instrument
		_iY -= SEQ_CELLS_YOFS;
		_iY /= SEQ_CELLH;
		
		// Get the new volume
		_iX -= (SEQ_XOFS+2);

		int iNewVol = (_iX * MAX_INST_VOL) / SEQ_MAX_VOLWIDTH;
		if (iNewVol != globals.m_oInstTab[_iY].m_ucVol)
		{
			// Set the new volume
			globals.m_oInstTab[_iY].m_ucVol = iNewVol;

			// Draw the new volume
			DrawVolumeLine(_iY,true);
		}

		CSoundSys::Play(FEEDBACKSOUND);
	}
}
//---------------------------------------------------------------------------
