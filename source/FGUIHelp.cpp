//---------------------------------------------------------------------------
#include <pa9.h>
#include "FGUIHelp.h"
#include "FApplicationGlobals.h"
#include "all_gfx.h"
#include "FSoundSys.h"
//---------------------------------------------------------------------------
const float SPLASH_SCREEN_TIME  = 4.0f;
static int  LAST_PAGE			= 0;
//---------------------------------------------------------------------------
static void* gspHELPPAGES[] = 
{
	(void*)pagebas0_Bitmap,
	(void*)pagebas1_Bitmap,
	(void*)pagebas2_Bitmap,
	(void*)pagebas3_Bitmap,

	(void*)pageMet0_Bitmap,
	(void*)pageMet1_Bitmap,
	(void*)pageMet2_Bitmap,
	(void*)pageMet3_Bitmap,
	(void*)pageMet3b_Bitmap,
	(void*)pageMet4_Bitmap,
	(void*)pageMet5_Bitmap,
	(void*)pageMet6_Bitmap,
	(void*)pageMet7_Bitmap,
	(void*)pageMet8_Bitmap,

	(void*)pageSeq0_Bitmap,
	(void*)pageSeq1_Bitmap,
	(void*)pageSeq2_Bitmap,
	(void*)pageSeq3_Bitmap,
	(void*)pageSeq4_Bitmap,
	(void*)pageSeq5_Bitmap,
	NULL,						// we can compute
};
//---------------------------------------------------------------------------
CGUIHelp::CGUIHelp()
{
	m_iCurPage = 0;
	m_fTime    = 0;
	
	// Compute the number of help pages
	LAST_PAGE = 0;
	while (gspHELPPAGES[LAST_PAGE])
		LAST_PAGE++;

	LAST_PAGE--;
}
//---------------------------------------------------------------------------
CGUIHelp::~CGUIHelp()
{
}
//---------------------------------------------------------------------------
void CGUIHelp::Init()
{
	
}
//---------------------------------------------------------------------------
void CGUIHelp::CursorDown(int _iX,int _iY,TCursorButton _eButton)
{
	if ((_iX>=26) && (_iX<=232) && (_iY>=40) && (_iY<=146))
	{
		switch (_eButton)
		{
			case CB_LEFT:
			{
				if (m_iCurPage < LAST_PAGE)
				{
					m_iCurPage++;
					
					CSoundSys::Play(FEEDBACKSOUND);
					Render();
				}
			}
			break;

			case CB_RIGHT:
			{
				if (m_iCurPage > 1)
				{
					m_iCurPage--;
					
					CSoundSys::Play(FEEDBACKSOUND);
					Render();
				}
			}
			break;
			default:
			{
			
			};
		}
	}
}
//---------------------------------------------------------------------------
void CGUIHelp::Render()
{
	if (! m_bVisible) return;

	PA_Init16bitBg(m_iScreen, 3);
	// PA_Clear16bitBg(m_iScreen);	
	PA_Load16bitBitmap( m_iScreen, ((unsigned short*)gspHELPPAGES[m_iCurPage]) );

	// Load a custom text font on the top screen
	PA_InitCustomText(m_iScreen,0,font);	// font name
	
	// Clear all the text on that screen
	PA_ClearTextBg(m_iScreen);
}
//---------------------------------------------------------------------------
void CGUIHelp::Update(float _fDeltaT)
{
	if (! m_bVisible) return;

	if (m_iCurPage == 0)
	{
		m_fTime	+= _fDeltaT;
		if (m_fTime > SPLASH_SCREEN_TIME)
		{
			// Simulate a cursordown event
			CursorDown(128,96,CB_LEFT);
		}
	}

	if (m_iCurPage>0)
		PA_OutputText(m_iScreen,15,6,"%d",m_iCurPage);
}
//---------------------------------------------------------------------------
