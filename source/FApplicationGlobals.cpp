#include "FApplicationGlobals.h"
#include <stdlib.h>			// For NULL definition
#include "FGUIMetro.h"
#include "FGUISeq.h"
//---------------------------------------------------------------------------
TApplicationGlobals globals =
{
	// Initial instrument sound parameters
	{
		{127,64,10,10},			// Bassdrum
		{ 96,48,10,10},			// Snare
		{ 48,96,10,10},			// Close Hihat
		{ 16,64,10,10},			// Open HiHat
		{ 32,80,10,10},			// Crash
		{127,64,10,10},			// Sticks
	},

	// Initial sequencer pattern
	{
		"1000100010001000",		// Bassdrum
		"0000100000001000",		// Snare
		"1010101010101010",		// Close Hihat
		"0010001000100010",		// Open HiHat
		"1000000000000000",		// Crash
		"0000000000000000",		// Sticks		
	},

	false,						// Initialized
	120,						// BPMs
	4,							// TempoNum
	4,							// TempoDen		

	// Runtime vars
	true,						// FirstTime of application execution
	false,						// Enabled
	-1,							// CurCol
	0.0f,						// 
	true,						// StylusJustPressed
	
	{GUIID_SEQ,GUIID_METRO},	// GUIs	
	{NULL,NULL,NULL},			// Stores the all GUIs used by the application

	0,							// Ticks since start of the app

	// For tapping tempo		
	0,							// Last Tapping Tick
	{0,0,0,0,0,0},				// Array of deltas between taps
	0,							// Current index in the delta array
	0,							// Current number of taps performed
	
	0,							// Current DeltaT
};
//---------------------------------------------------------------------------
void APP_IncreaseBPMs(int _iInc)
{
	globals.m_iBPMs += _iInc;

	if (globals.m_iBPMs < MIN_BPMS) globals.m_iBPMs = MIN_BPMS;
	if (globals.m_iBPMs > MAX_BPMS) globals.m_iBPMs = MAX_BPMS;
	
	((CGUIMetro*)globals.m_poGUI[GUIID_METRO])->SetBPMs(globals.m_iBPMs);
}
//---------------------------------------------------------------------------

	// Check del numerador: 
	/*
	Si cambio:
	- x cambiar valor globals.m_iNumTempo,
	- x Actualizar sprite para mostrar nuevo valor
	- x forzar render en secuenciador para mostrar nuevo tamaño de cuadrícula
	
	- TODO:
	- tener en cuenta el numerador a la hora de pintar la cuadrícula del secuenciador
	- tener en cuenta el numerador para capturar inputs de cursor en la cuadríacula del secuenciador!
	- tener en cuenta el numerador a la hora de reproducir el secuenciador
	*/

void APP_IncreaseTempoNum(int _iInc)
{
	if (_iInc > 0)
	{
		char ucNewNum[5] = {0,2,3,4,1};
		globals.m_iTempoNum = ucNewNum[globals.m_iTempoNum];
	}
	else
	{
		char ucNewNum[5] = {0,4,1,2,3};
		globals.m_iTempoNum = ucNewNum[globals.m_iTempoNum];
	}
	
	//
	((CGUIMetro*)globals.m_poGUI[GUIID_METRO])->SetTempoNum(globals.m_iTempoNum);
	
	// 
	if (globals.m_bEnabled)
	{
		// Restart metronome
		globals.m_iCurCol = -1;
		globals.m_fTime   = 0.0f;
	}

	//
	globals.m_poGUI[GUIID_SEQ]->Render();
}
//---------------------------------------------------------------------------
void APP_StopMetronome()
{
	globals.m_bEnabled = false;

	if(globals.m_poGUI[ globals.m_iScreenGUIs[UPPERSCREEN] ])
		globals.m_poGUI[ globals.m_iScreenGUIs[UPPERSCREEN] ]->Enable(false);

	if (globals.m_poGUI[ globals.m_iScreenGUIs[LOWERSCREEN] ])
		globals.m_poGUI[ globals.m_iScreenGUIs[LOWERSCREEN] ]->Enable(false);
}
//---------------------------------------------------------------------------
void APP_StartMetronome()
{
	globals.m_bEnabled = true;
	globals.m_iCurCol = -1;
	globals.m_fTime   = 0.0f;

	globals.m_poGUI[ globals.m_iScreenGUIs[UPPERSCREEN] ]->Enable(true);
	globals.m_poGUI[ globals.m_iScreenGUIs[LOWERSCREEN] ]->Enable(true);
}
//---------------------------------------------------------------------------
void APP_SetupGUIScreens()
{
	// Set the proper screen for the GUIs
	globals.m_poGUI[ globals.m_iScreenGUIs[UPPERSCREEN] ]->SetScreen(UPPERSCREEN);
 	globals.m_poGUI[ globals.m_iScreenGUIs[LOWERSCREEN] ]->SetScreen(LOWERSCREEN);
}
//---------------------------------------------------------------------------
