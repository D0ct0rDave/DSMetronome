//---------------------------------------------------------------------------
#include "FGUIMetro.h"
#include "FGUIHelp.h"
#include <PA9.h>
#include <nds.h>

#include "FApplicationGlobals.h"
#include "all_gfx.h"
#include "FSoundSys.h"
//---------------------------------------------------------------------------
#define METRO_BPMXOFS			126
#define METRO_BPMYOFS			80
#define METRO_CHARWIDTH			24

#define METRO_NUMXOFS			47
#define METRO_NUMYOFS			53

#define METRO_ONICONXOFS		51
#define METRO_ONICONYOFS		82

#define METRO_OFFICONXOFS		51
#define METRO_OFFICONYOFS		96

#define METRO_HELPICONXOFS		173
#define METRO_HELPICONYOFS		57

#define METRO_CENTS_SPRITE		0
#define METRO_DECS_SPRITE		1
#define METRO_UNITS_SPRITE		2
#define METRO_TEMPO_SPRITE		3
#define METRO_HELP_SPRITE		4
#define METRO_ONOFF_SPRITE		5

#define METRO_TEMPO_SPRITE_PAL	0
#define METRO_BPMS_SPRITE_PAL	1
#define METRO_ICONS_SPRITE_PAL	2
//---------------------------------------------------------------------------
void CGUIMetro::Init()
{
}
//---------------------------------------------------------------------------
void CGUIMetro::SetScreen(int _iScreen)
{
	if (_iScreen == -1)		   return;
	if (_iScreen == m_iScreen) return;

	if (m_iScreen != -1)
	{
		// First creation
		PA_DeleteSprite(m_iScreen,METRO_CENTS_SPRITE);
		PA_DeleteSprite(m_iScreen,METRO_DECS_SPRITE);
		PA_DeleteSprite(m_iScreen,METRO_UNITS_SPRITE);
		
		PA_DeleteSprite(m_iScreen,METRO_TEMPO_SPRITE);

		PA_DeleteSprite(m_iScreen,METRO_ONOFF_SPRITE);
		PA_DeleteSprite(m_iScreen,METRO_HELP_SPRITE);
	}
	
	m_iScreen = _iScreen;

	// Display bpms
	PA_CreateSprite (m_iScreen, METRO_CENTS_SPRITE,(void*)numbers_Sprite, OBJ_SIZE_32X32,1, METRO_BPMS_SPRITE_PAL,METRO_BPMXOFS                  , METRO_BPMYOFS);
	PA_CreateSprite (m_iScreen, METRO_DECS_SPRITE, (void*)numbers_Sprite, OBJ_SIZE_32X32,1, METRO_BPMS_SPRITE_PAL,METRO_BPMXOFS+METRO_CHARWIDTH*1, METRO_BPMYOFS);
	PA_CreateSprite (m_iScreen, METRO_UNITS_SPRITE,(void*)numbers_Sprite, OBJ_SIZE_32X32,1, METRO_BPMS_SPRITE_PAL,METRO_BPMXOFS+METRO_CHARWIDTH*2, METRO_BPMYOFS);

	PA_CreateSprite (m_iScreen, METRO_TEMPO_SPRITE,(void*)numbers2_Sprite,OBJ_SIZE_32X32,1, METRO_TEMPO_SPRITE_PAL,METRO_NUMXOFS				 , METRO_NUMYOFS);

	PA_CreateSprite (m_iScreen, METRO_ONOFF_SPRITE,(void*)icons_Sprite  , OBJ_SIZE_32X16,1, METRO_ICONS_SPRITE_PAL,METRO_ONICONXOFS				 , METRO_ONICONYOFS);
	PA_CreateSprite (m_iScreen, METRO_HELP_SPRITE ,(void*)icons_Sprite  , OBJ_SIZE_32X16,1, METRO_ICONS_SPRITE_PAL,METRO_HELPICONXOFS			 , METRO_HELPICONYOFS);
	PA_SetSpriteAnim(m_iScreen, METRO_HELP_SPRITE ,2);

	PA_LoadSpritePal(m_iScreen, METRO_BPMS_SPRITE_PAL, (void*)numbers_Pal );
	PA_LoadSpritePal(m_iScreen, METRO_TEMPO_SPRITE_PAL,(void*)numbers2_Pal);
	PA_LoadSpritePal(m_iScreen, METRO_ICONS_SPRITE_PAL,(void*)icons_Pal   );
	

	// Setup BPMs (for sprite frame indexes)
	SetupBPMSprites();

	// Setup Tempo numerator sprite
	SetupTempoNumSprite();
	SetupOnOffSprite();

	// Display the full screen
	Render();
}
//---------------------------------------------------------------------------
void CGUIMetro::SetupBPMSprites()
{
	if (m_iScreen == -1) return;

	// Centenas 
	int iCents = globals.m_iBPMs / 100;
	int iDec   = (globals.m_iBPMs % 100) / 10;
	int iUnits = (globals.m_iBPMs % 10);
	
	if (iCents)
		PA_SetSpriteAnim(m_iScreen,METRO_CENTS_SPRITE,iCents);
	else
		PA_SetSpriteAnim(m_iScreen,0,10);

	PA_SetSpriteAnim(m_iScreen,METRO_DECS_SPRITE,iDec);
	PA_SetSpriteAnim(m_iScreen,METRO_UNITS_SPRITE,iUnits);
}
//---------------------------------------------------------------------------
void CGUIMetro::SetupTempoNumSprite()
{
	if (m_iScreen == -1) return;
	PA_SetSpriteAnim(m_iScreen,METRO_TEMPO_SPRITE,globals.m_iTempoNum-1);
}
//---------------------------------------------------------------------------
void CGUIMetro::SetupOnOffSprite()
{	
	if (m_iScreen == -1) return;
	
	if (globals.m_bEnabled)
	{
		PA_SetSpriteAnim(m_iScreen,METRO_ONOFF_SPRITE,0);
		PA_SetSpriteXY(m_iScreen,METRO_ONOFF_SPRITE,METRO_ONICONXOFS,METRO_ONICONYOFS);
	}
	else
	{		
		PA_SetSpriteAnim(m_iScreen,METRO_ONOFF_SPRITE,1);
		PA_SetSpriteXY(m_iScreen,METRO_ONOFF_SPRITE,METRO_OFFICONXOFS,METRO_OFFICONYOFS);
	}
}
//---------------------------------------------------------------------------
void CGUIMetro::SetBPMs(int _iBPMs)
{
	SetupBPMSprites();
}
//---------------------------------------------------------------------------
void CGUIMetro::SetTempoNum(int _iTempoNum)
{
	SetupTempoNumSprite();
}
//---------------------------------------------------------------------------
void CGUIMetro::Enable(bool _bEnable)
{
	CGUI::Enable(_bEnable);
	SetupOnOffSprite();
}
//---------------------------------------------------------------------------
void CGUIMetro::Render()
{
	if (m_iScreen == -1) return;
	if (! m_bVisible) return;

	PA_Init16bitBg(m_iScreen, 3);	// Order is important!
	PA_Clear16bitBg(m_iScreen);
	PA_Load16bitBitmap(m_iScreen,screen1_Bitmap);

	// Clear all the text on that screen
	PA_InitText(m_iScreen, 0);
	PA_ClearTextBg(m_iScreen);		
}
//---------------------------------------------------------------------------
void CGUIMetro::CursorDown(int _iX,int _iY,TCursorButton _eButton)
{
	// Cents
	if (PA_SpriteTouched(METRO_CENTS_SPRITE))
	{
		CSoundSys::Play(FEEDBACKSOUND);

	    if (_eButton == CB_LEFT)
			APP_IncreaseBPMs(100);
   else if (_eButton == CB_RIGHT)
			APP_IncreaseBPMs(-100);
	}

	// Dec
else if (PA_SpriteTouched(METRO_DECS_SPRITE))
	{
		CSoundSys::Play(FEEDBACKSOUND);
	
	    if (_eButton == CB_LEFT)
			APP_IncreaseBPMs(10);
   else if (_eButton == CB_RIGHT)	
			APP_IncreaseBPMs(-10);
	}
	
	// Units
else if (PA_SpriteTouched(METRO_UNITS_SPRITE))
	{
		CSoundSys::Play(FEEDBACKSOUND);
					
	    if (_eButton == CB_LEFT)
			APP_IncreaseBPMs(1);
   else if (_eButton == CB_RIGHT)	
			APP_IncreaseBPMs(-1);
	}
	
	// Tempo Numerator	
else if (PA_SpriteTouched(METRO_TEMPO_SPRITE))
	{
		CSoundSys::Play(FEEDBACKSOUND);

	    if (_eButton == CB_LEFT)
			APP_IncreaseTempoNum(1);
	else if (_eButton == CB_RIGHT)
			APP_IncreaseTempoNum(-1);
	}

	// Help button
else if (PA_SpriteTouched(METRO_HELP_SPRITE))
	{
		CSoundSys::Play(FEEDBACKSOUND);

		if (globals.m_poGUI[GUIID_HELP]->bVisible() )
		{
			globals.m_poGUI[GUIID_HELP]->Hide();
			globals.m_poGUI[GUIID_SEQ ]->Show();
			
			globals.m_iScreenGUIs[UPPERSCREEN] = GUIID_SEQ;	// Should be the upper since we're here due an input in the bottom screen...
		}
		else
		{
			globals.m_poGUI[GUIID_HELP]->Show();
			globals.m_poGUI[GUIID_SEQ ]->Hide();

			globals.m_iScreenGUIs[UPPERSCREEN] = GUIID_HELP;
		}
	
		APP_SetupGUIScreens();
	}
}
//---------------------------------------------------------------------------
