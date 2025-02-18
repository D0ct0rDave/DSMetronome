//---------------------------------------------------------------------------
#ifndef FApplicationGlobalsH
#define FApplicationGlobalsH
//---------------------------------------------------------------------------
#include <jtypes.h>
//---------------------------------------------------------------------------
typedef enum TCursorButton{

	CB_NONE,
	CB_LEFT,
	CB_RIGHT,
	CB_NUMBUTTONS
};
//---------------------------------------------------------------------------
#define UPPERSCREEN 1
#define LOWERSCREEN	0
//---------------------------------------------------------------------------
typedef enum TGUIID{
	GUIID_NONE,
	GUIID_SEQ,
	GUIID_METRO,
	GUIID_HELP,
	GUIID_NUMGUIIDS
};
//---------------------------------------------------------------------------
#define MAX_INSTRUMENTS		6
#define FEEDBACKSOUND		MAX_INSTRUMENTS

#define MAX_TICS			16
#define TICS_PER_BEAT		MAX_TICS / 4;


#define MIN_BPMS				40
#define MAX_BPMS				250
#define MAX_MILIS_BETWEEN_TAPS	(60000 / MIN_BPMS)

#define MAX_INST_VOL			127

//---------------------------------------------------------------------------
#define FONT_COLOR PA_RGB(31,18,0)
//---------------------------------------------------------------------------
typedef struct{

	unsigned char   m_ucVol;
	unsigned char	m_ucPan;
	
	unsigned char   m_ucVolRnd;
	unsigned char	m_ucPanRnd;

}TInstrumentInfo;
//---------------------------------------------------------------------------
#define NUM_TAPS		4					// Minimum number TAPS needed for the user to recognise a tempo
#define MAX_DELTAS		6					// Size of the buffer to perform the tempo calculation
//---------------------------------------------------------------------------
class CGUI;

typedef struct TApplicationGlobals
{
	TInstrumentInfo m_oInstTab[MAX_INSTRUMENTS];

	char* 			m_ucPatternGrid[MAX_INSTRUMENTS];

	int   			m_iInitialized;	
	int   			m_iBPMs;
	int   			m_iTempoNum;
	int   			m_iTempoDen;

	// Runtime vars
	bool			m_bFirstTime;
	bool  			m_bEnabled;
	int	  			m_iCurCol;

	float 			m_fTime;
	bool  			m_bStylusJustPressed;
	int   			m_iScreenGUIs[2];			// Stores the GUI ID displayed in everyone of the DS Screens
	CGUI* 			m_poGUI[GUIID_NUMGUIIDS];	// Stores the all GUIs used by the application
	//
	unsigned int	m_uiTicks;					// Ticks since the start of the application

	// For tapping tempo
	unsigned int	m_uiLastTapTics;			// Last time user performed a tap
	unsigned int	m_uiDeltas[MAX_DELTAS];		// Array of deltas to average the tempo
	u8				m_uiTapIdx;					// Index into the delta array
	u8				m_iNumTaps;					// Current number of taps performed
	
	float			m_fDeltaT;

}TApplicationGlobals;

//---------------------------------------------------------------------------

extern TApplicationGlobals globals;
//---------------------------------------------------------------------------
void APP_SetupGUIScreens();
void APP_StartMetronome();
void APP_StopMetronome();
void APP_IncreaseBPMs(int _iInc);
void APP_IncreaseTempoNum(int _iInc);
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
