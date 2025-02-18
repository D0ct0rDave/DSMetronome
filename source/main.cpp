// Includes
#include <fat.h>
#include <PA9.h>

#include "FApplicationGlobals.h"
#include "FGUISeq.h"
#include "FGUIMetro.h"
#include "FGUIHelp.h"

#include "FSoundSys.h"

#include "all_gfx.h"
//---------------------------------------------------------------------------
#define USE_TIMER
#ifdef USE_TIMER
static unsigned int MAIN_TIMER	= 0;
#endif
//---------------------------------------------------------------------------
#define CONFIGFILE "DSMetronome.dat"
//---------------------------------------------------------------------------
#define BEGIN_LOADDATA()\
	int iOfs = 0;\
	FILE* fd= fopen(CONFIGFILE,"rb");

#define END_LOADDATA()\
	if (fd != NULL)\
		fclose(fd);

#define LOADDATA(data,size)\
{\
	if (fd != NULL)\
		fread(data,(size),1,fd);\
	else\
	{\
		PA_LoadData (iOfs,data,(size));\
		iOfs += (size);\
	}\
}

#define BEGIN_SAVEDATA()\
	int iOfs = 0;\
	FILE* fd= fopen(CONFIGFILE,"wb");

#define END_SAVEDATA()\
	if (fd != NULL)\
		fclose(fd);

#define SAVEDATA(data,size)\
{\
	if (fd != NULL)\
		fwrite(data,(size),1,fd);\
	else\
	{\
		PA_SaveData (iOfs,data,(size));\
		iOfs += (size);\
	}\
}
//---------------------------------------------------------------------------
void APP_LoadData()
{
	BEGIN_LOADDATA();
	
		LOADDATA(&globals.m_iInitialized,sizeof(int));
		if (globals.m_iInitialized != 1)
		{
			// if we do not exit, we're going to overwrite all the statically initialized data
			END_LOADDATA();
			return;
		}

		LOADDATA(&globals.m_iBPMs,sizeof(int));
		LOADDATA(&globals.m_iTempoNum,sizeof(int));
		LOADDATA(&globals.m_iTempoDen,sizeof(int));
		LOADDATA(globals.m_oInstTab,sizeof(TInstrumentInfo)*MAX_INSTRUMENTS);

		for (int i=0;i<MAX_INSTRUMENTS;i++)
			LOADDATA(globals.m_ucPatternGrid[i],MAX_TICS);
	
	END_LOADDATA();
}
//---------------------------------------------------------------------------
void APP_SaveData()
{
	BEGIN_SAVEDATA();
		
		SAVEDATA(&globals.m_iInitialized,sizeof(int));
		SAVEDATA(&globals.m_iBPMs,sizeof(int));
		SAVEDATA(&globals.m_iTempoNum,sizeof(int));
		SAVEDATA(&globals.m_iTempoDen,sizeof(int));
		SAVEDATA(globals.m_oInstTab,sizeof(TInstrumentInfo)*MAX_INSTRUMENTS);

		for (int i=0;i<MAX_INSTRUMENTS;i++)
			SAVEDATA(globals.m_ucPatternGrid[i],MAX_TICS);

	END_SAVEDATA();
}
//---------------------------------------------------------------------------
void APP_Init()
{
	// Clear everything
	// memset(&globals,0,sizeof(globals));
	
	int result = fatInitDefault();
	
	// Load the data
	APP_LoadData();

	if (globals.m_iInitialized != 1)
	{
		globals.m_iInitialized = 1;
		APP_SaveData();
	}
	else	
	{
		globals.m_bFirstTime = false;
	}

	// Init sound	
	CSoundSys::Init();

	// Init the timer
	#ifdef USE_TIMER
		StartTime(true);				// Start timing system
		MAIN_TIMER = NewTimer(true);	// Create a timer
		StartTimer(MAIN_TIMER);			// Start this timer
		globals.m_uiTicks  = Tick(MAIN_TIMER);
	#else
		globals.m_uiTicks  = 0;
	#endif


	// Init the metronome	
	globals.m_bEnabled = true;
}
//---------------------------------------------------------------------------
void APP_InitGUI()
{
	globals.m_poGUI[GUIID_SEQ  ]		= new CGUISeq;
	globals.m_poGUI[GUIID_METRO]		= new CGUIMetro;
	globals.m_poGUI[GUIID_HELP]			= new CGUIHelp;

	for (int i=0;i<GUIID_NUMGUIIDS;i++)	
		if (globals.m_poGUI[i])
			globals.m_poGUI[i]->Init();

	((CGUIMetro*)globals.m_poGUI[GUIID_METRO])->SetBPMs(globals.m_iBPMs);

	globals.m_iScreenGUIs[LOWERSCREEN]	= GUIID_METRO;
	globals.m_iScreenGUIs[UPPERSCREEN]	= globals.m_bFirstTime?GUIID_HELP:GUIID_SEQ; // GUIID_SEQ; //

	APP_SetupGUIScreens();

	// Show the GUIs
	globals.m_poGUI[ globals.m_iScreenGUIs[UPPERSCREEN] ]->Show();
	globals.m_poGUI[ globals.m_iScreenGUIs[LOWERSCREEN] ]->Show();
}
//---------------------------------------------------------------------------
void SwapScreens()
{
	// Swap GUIs / Screens
	int iAux = globals.m_iScreenGUIs[0];
	globals.m_iScreenGUIs[0] = globals.m_iScreenGUIs[1];
	globals.m_iScreenGUIs[1] = iAux;

	APP_SetupGUIScreens();
}
//---------------------------------------------------------------------------
void APP_TimerTick(float _fDeltaT)
{
	int iCol = 0;
	if (globals.m_iCurCol != -1) // the first beat we should not take care of DeltaT
	{
		globals.m_fTime += _fDeltaT;

		int iMaxTics   = globals.m_iTempoNum * TICS_PER_BEAT;
		float fSecs    = globals.m_fTime;
		float fSBPSs   = (float)globals.m_iBPMs * 4.0f / 60.0f;

		float fSubBeat = fSecs * fSBPSs;
		int iSubBeat   = (int)(fSubBeat) % iMaxTics;
		
		iCol = iSubBeat;
	}

	if (iCol != globals.m_iCurCol)
	{
		globals.m_iCurCol = iCol;

		if (globals.m_poGUI[GUIID_SEQ])
			((CGUISeq*)globals.m_poGUI[GUIID_SEQ])->UpdateCursor(globals.m_iCurCol);

		for (int i=0;i<MAX_INSTRUMENTS;i++)
			if (globals.m_ucPatternGrid[i][iCol] == '1')
				CSoundSys::Play(i);
	}
}
//---------------------------------------------------------------------------
void APP_UpdateCursorInputs()
{
	// Check pointer actions
	PA_UpdateStylus();

	if (Stylus.Held)
	{
		if (globals.m_bStylusJustPressed)
		{					
			globals.m_bStylusJustPressed = false;

			// Simulate cursor left / cursor right with a shift key (L trigger)
			TCursorButton cb = CB_NONE;
			if (Pad.Held.L)
				cb = CB_RIGHT;
			else
				cb = CB_LEFT;

			// Send cursor down
			globals.m_poGUI[ globals.m_iScreenGUIs[LOWERSCREEN] ]->CursorDown(Stylus.X, Stylus.Y,cb);
		}
	}
	else
		globals.m_bStylusJustPressed = true;
}
//---------------------------------------------------------------------------
void APP_HandleTappingInput()
{
	if ((globals.m_uiTicks - globals.m_uiLastTapTics) > MAX_MILIS_BETWEEN_TAPS)
	{
		globals.m_iNumTaps = 0;
		globals.m_uiTapIdx = 0;
	}

	if (Pad.Newpress.B)
	{
		// Avoid becoming mad while tapping
		APP_StopMetronome();

		if (globals.m_iNumTaps > 0)
		{
			globals.m_uiDeltas[globals.m_uiTapIdx] = globals.m_uiTicks - globals.m_uiLastTapTics;
			globals.m_uiTapIdx                     = (globals.m_uiTapIdx+1) % MAX_DELTAS;
		}

		globals.m_uiLastTapTics = globals.m_uiTicks;

		if (globals.m_iNumTaps < NUM_TAPS)
		{
			globals.m_iNumTaps++;
		}
		else
		{
			// Setup new tapped tempo (average of deltas)
			unsigned int uiDeltasSum = 0;
			for (int i=0;i<globals.m_iNumTaps;i++)
				uiDeltasSum += globals.m_uiDeltas[i];

			uiDeltasSum /= globals.m_iNumTaps;
		
			globals.m_iBPMs = 60000 / uiDeltasSum;
			((CGUIMetro*)globals.m_poGUI[GUIID_METRO])->SetBPMs(globals.m_iBPMs);

			// New tap
			globals.m_iNumTaps++;
			if (globals.m_iNumTaps > MAX_DELTAS) globals.m_iNumTaps = MAX_DELTAS;			
		}
	}
}
//---------------------------------------------------------------------------
int main(int argc, char** argv)
{
	// PALib Initialization
	PA_Init();
	PA_InitVBL();	

	// Initialize application
	APP_Init();

	// Init GUIs
	APP_InitGUI();

	// Infinite loop to keep the program running
	unsigned int uiLastTicks = globals.m_uiTicks;
	while (1)
	{		
		// ---------------------------
		// Control inputs
		// ---------------------------
		// Check if we need to save the data
		if (Pad.Newpress.Start)
		{
			APP_SaveData();

			if (globals.m_bEnabled)
				APP_StopMetronome();
			else
				APP_StartMetronome();
		}

		if (Pad.Newpress.Up)
			APP_IncreaseBPMs( 10);
		if (Pad.Newpress.Down)
			APP_IncreaseBPMs(-10);
		if (Pad.Newpress.Left)
			APP_IncreaseBPMs( -1);
		if (Pad.Newpress.Right)
			APP_IncreaseBPMs(  1);

		// Check swap of screens
		if (Pad.Newpress.Select)
			SwapScreens();

		// Tempo tapping
		APP_HandleTappingInput();

		// Update cursor inputs
		APP_UpdateCursorInputs();

		// ---------------------------
		// Timing management
		// ---------------------------
		// We put timer management here due to the SaveDATA lag introduced which affects
		// the metronome

		#ifdef USE_TIMER
		globals.m_uiTicks = Tick(MAIN_TIMER);
		globals.m_fDeltaT = (float)(globals.m_uiTicks - uiLastTicks) / 1000.0f;
		#else
		globals.m_fDeltaT = (1.0f / 60.0f);
		globals.m_uiTicks += (int)(1000.0f*globals.m_fDeltaT);
		#endif

		// 
		if (globals.m_bEnabled)
			APP_TimerTick ( globals.m_fDeltaT );

		CSoundSys::Update();

		for (int i=0;i<GUIID_NUMGUIIDS;i++)
			if (globals.m_poGUI[i])
				globals.m_poGUI[i]->Update( globals.m_fDeltaT );

		PA_WaitForVBL();
		
		uiLastTicks = globals.m_uiTicks;
	}

	return 0;
}
//---------------------------------------------------------------------------
// OLDCODE
//---------------------------------------------------------------------------

