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
	

	bool GetMute(MixerDeive dev); // 检查设备是否静音
	bool SetMute(MixerDeive dev, bool vol);	//设置设备静音
	bool SetVolume(MixerDeive dev, long vol); //设置设备的音量
	unsigned GetVolume(MixerDeive dev); //得到设备的音量 dev = 0 主音量, 1WAVE, 2MIDI, 3 LINE IN
	bool GetMixerNumDevs();

private:
	bool GetVolumeControl(HMIXER hmixer, long componentType, long ctrlType, MIXERCONTROL *mxc);
	bool SetVolumeValue(HMIXER hmixer, MIXERCONTROL *mxc, long volume);
	bool SetMuteValue(HMIXER hmixer, MIXERCONTROL *mxc, bool mute);
	unsigned GetVolumeValue(HMIXER hmixer, MIXERCONTROL *mxc);
	long GetMuteValue(HMIXER hmixer, MIXERCONTROL *mxc);

	long m_VolRange;
};