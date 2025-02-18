//---------------------------------------------------------------------------
#ifndef FSoundH
#define FSoundH
//---------------------------------------------------------------------------
class CSoundSys
{
	public:
		static void Init();
		static void Play(int _iCh);
		static void Update();
		
	private:
		static void PlaySound(int _iChannel,void* _pData,int _iSize,int _iVolume,int _iPan);
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
