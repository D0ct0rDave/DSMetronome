#include "FSoundSys.h"
#include <PA9.h>
#include <stdlib.h>
// ----------------------------------------------------------------------------
#include "FApplicationGlobals.h"

#include "BassDrum.h"
#include "Snare.h"
#include "ClosedHiHat.h"
#include "OpenHihat.h"
#include "Crash.h"
// #include "Ride.h"
#include "Sticks.h"
#include "Click.h"
// ----------------------------------------------------------------------------
#define DEFSAMPLERATE 44100
#define SOUND_OPTION AS_SURROUND
// #define RESERVE_CHANNEL_PER_INSTRUMENT
// ----------------------------------------------------------------------------
/// sound info
/*
typedef struct
{
    u8  *data;
    u32 size;
    u8  format;
    s32  rate;
    u8  volume;
    s8  pan;
    u8  loop;
    u8  priority;
    u8  delay;
    
} SoundInfo;
*/

static SoundInfo gsoInstruments[MAX_INSTRUMENTS+1] =
{
	{(u8*)BassDrum,		BassDrum_size,		AS_PCM_16BIT,DEFSAMPLERATE, 0, 0,false,0,SOUND_OPTION},
	{(u8*)Snare,		Snare_size,			AS_PCM_16BIT,DEFSAMPLERATE, 0, 0,false,1,SOUND_OPTION},
	{(u8*)ClosedHiHat,	ClosedHiHat_size,	AS_PCM_16BIT,DEFSAMPLERATE, 0, 0,false,2,SOUND_OPTION},
	{(u8*)OpenHihat,	OpenHihat_size,		AS_PCM_16BIT,DEFSAMPLERATE, 0, 0,false,3,SOUND_OPTION},
	{(u8*)Crash,		Crash_size,			AS_PCM_16BIT,DEFSAMPLERATE, 0, 0,false,4,SOUND_OPTION},
	// {(u8*)Ride,			Ride_size,			AS_PCM_16BIT,DEFSAMPLERATE, 0, 0,false,4,SOUND_OPTION},	
	{(u8*)Sticks,		Sticks_size,		AS_PCM_16BIT,DEFSAMPLERATE, 0, 0,false,5,SOUND_OPTION},	
	{(u8*)Click,		Click_size,			AS_PCM_16BIT,DEFSAMPLERATE, 32,64,false,6,SOUND_OPTION},	
};
// ----------------------------------------------------------------------------
void CSoundSys::Init()
{
	AS_Init(AS_MODE_16CH);
	// PA_VBLFunctionInit(AS_SoundVBL);

	AS_SetDefaultSettings(AS_PCM_16BIT, DEFSAMPLERATE, SOUND_OPTION);
	AS_SetMasterVolume(127);
	for (int i=0;i<MAX_INSTRUMENTS;i++)
	{
		#ifdef RESERVE_CHANNEL_PER_INSTRUMENT
		AS_ReserveChannel(i);
		#endif
		
		AS_SetSoundVolume(i,127);
		AS_SetSoundPan(i,64);
	} // end for
}
// ----------------------------------------------------------------------------
int iSRand(int _iMax)
{
	return ( (rand() % (2*_iMax)) - _iMax );
}
// ----------------------------------------------------------------------------
void CSoundSys::Play(int _iCh)
{
	if (_iCh == FEEDBACKSOUND)
	{
		AS_SoundPlay( gsoInstruments[_iCh] );
		return;
	}

	// Add some randomness to volume
	int iMainVol = globals.m_oInstTab[_iCh].m_ucVol;
	int iVol    = iMainVol + (iMainVol * iSRand(globals.m_oInstTab[_iCh].m_ucVolRnd) / 100);
	if (iVol > 127) iVol = 127; else if (iVol < 0) return;

	// Add some randomness to pane
	int iMainPan = globals.m_oInstTab[_iCh].m_ucPan;
	int iPan    = iMainPan + (iMainPan * iSRand(globals.m_oInstTab[_iCh].m_ucPanRnd) / 100);
	if (iPan > 127) iPan = 127; else if (iPan < 0) iPan = 0;

	// Setup instrument parameters
	gsoInstruments[_iCh].volume = iVol;
	gsoInstruments[_iCh].pan    = iPan;
	
	#ifdef RESERVE_CHANNEL_PER_INSTRUMENT

		// play a sound directly using the given channel	
		// DMC: No se porqué, pero no funciona en una DS real ... :( en el DeSmume si
		AS_SoundStop(_iCh);
		AS_SoundDirectPlay(_iCh,gsoInstruments[_iCh]);

	#else
		AS_SoundPlay(gsoInstruments[_iCh]);
	#endif
}
// ----------------------------------------------------------------------------
void CSoundSys::Update()
{
	return;

	int i;
	for (i = 0; i < 16; i++)
		PA_OutputText(0, 0, i, "Channel %d busy : %d  ", i, PA_SoundChannelIsBusy(i));	 // 0 for free, 1 for busy
}
// ----------------------------------------------------------------------------

