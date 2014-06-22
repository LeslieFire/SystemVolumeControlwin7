#pragma once

#include "stdafx.h"
#include <Windows.h>
#include <MMSystem.h>
#pragma comment(lib, "winmm.lib")

class Mixer{
public:
	Mixer();
	Mixer(const int volRange);
	virtual ~Mixer();
	
	enum MixerDeive{
		SPEAKERS = 0,
		WAVEOUT,
		SYNTHESIZER,
		MICROPHONE
	};
	

	bool GetMute(MixerDeive dev); // ����豸�Ƿ���
	bool SetMute(MixerDeive dev, bool vol);	//�����豸����
	bool SetVolume(MixerDeive dev, long vol); //�����豸������
	unsigned GetVolume(MixerDeive dev); //�õ��豸������ dev = 0 ������, 1WAVE, 2MIDI, 3 LINE IN
	bool GetMixerNumDevs();

private:
	bool GetVolumeControl(HMIXER hmixer, long componentType, long ctrlType, MIXERCONTROL *mxc);
	bool SetVolumeValue(HMIXER hmixer, MIXERCONTROL *mxc, long volume);
	bool SetMuteValue(HMIXER hmixer, MIXERCONTROL *mxc, bool mute);
	unsigned GetVolumeValue(HMIXER hmixer, MIXERCONTROL *mxc);
	long GetMuteValue(HMIXER hmixer, MIXERCONTROL *mxc);

	long m_VolRange;
};