#include "stdafx.h"
#include "Mixer.h"

Mixer::Mixer(){
	m_VolRange = 100;
}
Mixer::~Mixer(){}

Mixer::Mixer(const int VolRange){
	m_VolRange = VolRange;
}

//------------------set volume-----------------
bool Mixer::GetVolumeControl(HMIXER hmixer, long componentType, long ctrlType, MIXERCONTROL *mxc){
	MIXERLINECONTROLS mxlc;
	MIXERLINE mxl;
	MMRESULT  err;
	bool exist = false;

	mxl.cbStruct = sizeof(MIXERLINE);
	mxl.dwComponentType = componentType;

	if (componentType == MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE){
		// 获取录音microphone设备
		mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;
		// 得到录制总线中的链接数
		mixerGetLineInfo((HMIXEROBJ)hmixer, &mxl, 
			MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_COMPONENTTYPE);
		// 将连接数保存
		DWORD dwConnections = mxl.cConnections;
		// 准备获取microphone设备的ID
		DWORD dwLineID = 0;
		for ( DWORD i = 0; i < dwConnections; ++i){
			// 枚举每一个设备，当Source的ID 等于当前的迭代计数
			mxl.dwSource = i;
			// 根绝SourceID获得连接的信息
			MMRESULT mr = mixerGetLineInfo( (HMIXEROBJ)hmixer, &mxl, 
				MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_SOURCE);
			// 判断函数执行错误
			if (mr != 0){
				break;
			}

			// 如果当前设备类型是microphone， 则跳出循环
			if ( mxl.dwComponentType == MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE){
				exist = true;
				break;
			}
		}
	} else if (!(err=mixerGetLineInfo((HMIXEROBJ)hmixer, &mxl, MIXER_GETLINEINFOF_COMPONENTTYPE))){
		exist = true;
		cout << "Get volume control exist = true" <<endl;
	}

	if (exist){
		mxlc.cbStruct = sizeof(mxlc);
		mxlc.dwLineID = mxl.dwLineID;
		mxlc.dwControlType = ctrlType;
		mxlc.cControls = 1;
		mxlc.cbmxctrl = sizeof(MIXERCONTROL);
		mxlc.pamxctrl = mxc;
		if((err = mixerGetLineControls((HMIXEROBJ)hmixer, &mxlc, MIXER_GETLINECONTROLSF_ONEBYTYPE))){
			printf("Error #%d calling mixerGetLineControl() \n", err);
			return 0;
		}else {
			printf("Right #%d calling mixerGetLineControl() \n", err);
			return 1;
		}		
	}
	return 0;
}

long Mixer::GetMuteValue(HMIXER hmixer, MIXERCONTROL *mxc){
	MIXERCONTROLDETAILS mxcd;
	MIXERCONTROLDETAILS_BOOLEAN mxcdMute;
	mxcd.hwndOwner = 0;
	mxcd.cbStruct = sizeof(mxcd);
	mxcd.dwControlID = mxc->dwControlID;
	mxcd.cbDetails = sizeof(mxcdMute);
	mxcd.paDetails = &mxcdMute;
	mxcd.cChannels = 1;
	mxcd.cMultipleItems = 0;
	if(mixerGetControlDetails((HMIXEROBJ)hmixer, &mxcd, MIXER_OBJECTF_HMIXER | MIXER_GETCONTROLDETAILSF_VALUE))
		return -1;
	return mxcdMute.fValue;
}

unsigned Mixer::GetVolumeValue(HMIXER hmixer, MIXERCONTROL *mxc){
	MIXERCONTROLDETAILS mxcd;
	MIXERCONTROLDETAILS_UNSIGNED vol;
	MMRESULT	err;
	vol.dwValue  = 0;
	mxcd.hwndOwner = 0;
	mxcd.cbStruct = sizeof(mxcd);
	mxcd.dwControlID = mxc->dwControlID;
	mxcd.cbDetails = sizeof(vol);
	mxcd.paDetails = &vol;
	mxcd.cChannels = 1;
	if((err = mixerGetControlDetails((HMIXEROBJ)hmixer, &mxcd, /*MIXER_OBJECTF_HMIXER | */MIXER_GETCONTROLDETAILSF_VALUE))){
		printf("Error #%d calling mixerGetControlDetails()\n", err);
		return -1;
	}else{
		printf("It's value is %lu\n", vol.dwValue);
	}
		
	return vol.dwValue;
}

bool Mixer::SetMuteValue(HMIXER hmixer, MIXERCONTROL *mxc, bool mute){
	MIXERCONTROLDETAILS mxcd;
	MIXERCONTROLDETAILS_BOOLEAN mxcdMute;
	MMRESULT err;
	mxcdMute.fValue = mute;
	mxcd.hwndOwner = 0;
	mxcd.dwControlID = mxc->dwControlID;
	mxcd.cbStruct = sizeof(mxcd);
	mxcd.cbDetails = sizeof(mxcdMute);
	mxcd.paDetails = &mxcdMute;
	mxcd.cChannels = 1;
	mxcd.cMultipleItems = 0;
	if (err = mixerSetControlDetails((HMIXEROBJ)hmixer, &mxcd, MIXER_OBJECTF_HMIXER|MIXER_SETCONTROLDETAILSF_VALUE)){
		printf("ERROR : SetMuteValue: mixSetControlDetails-> %s\n", err);
		return 0;
	}

	return 1;
}

bool Mixer::SetVolumeValue(HMIXER hmixer, MIXERCONTROL *mxc, long volume){
	MIXERCONTROLDETAILS mxcd;
	MIXERCONTROLDETAILS_UNSIGNED vol;
	vol.dwValue = volume;
	mxcd.hwndOwner = 0;
	mxcd.dwControlID = mxc->dwControlID;
	mxcd.cbStruct = sizeof(mxcd);
	mxcd.cbDetails = sizeof(vol);
	mxcd.paDetails = &vol;
	mxcd.cChannels = 1;
	if (mixerSetControlDetails((HMIXEROBJ)hmixer, &mxcd, MIXER_OBJECTF_HMIXER|MIXER_SETCONTROLDETAILSF_VALUE))
		return 0;
	return 1;
}
//-------------------WINAPI----------------
unsigned Mixer::GetVolume(MixerDeive dev){
	long device;
	unsigned rt = 0;
	MIXERCONTROL volCtrl;
	HMIXER hmixer;
	switch(dev){
	case WAVEOUT:
		device = MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT; 
		printf("GetVolume: device = MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT\n");
		break;
	case SYNTHESIZER:
		device = MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER;
		printf("GetVolume: device = MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER\n");
		break;
	case MICROPHONE:
		// device = MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC; 
		device = MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE;
		// device = MIXERLINE_COMPONENTTYPE_SRC_LINE;
		// device = MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC;
		printf("GetVolume: device = MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE\n");
		break;

	default:
		device=MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
		printf("GetVolume: device = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS\n");
	}

	if(mixerOpen(&hmixer, 0, 0, 0, 0)) return 0;
	if(!GetVolumeControl(hmixer, device, MIXERCONTROL_CONTROLTYPE_VOLUME, &volCtrl)){
		
		mixerClose(hmixer);
		return 0;
	}

	rt = GetVolumeValue(hmixer, &volCtrl) * m_VolRange / volCtrl.Bounds.lMaximum;
	mixerClose(hmixer);
	return rt;
}

//设置设备音量
bool Mixer::SetVolume(MixerDeive dev, long vol){
	// dev = 0, 1, 2 分别表示主音量， 波形， MIDI， LINE IN
	// vol = 0~m_volRange 表示音量的大小， 设置于返回音量的值用的是百分比，即音量从0 ~ m_volRange，
	// 而不是设备的绝对值
	// return false 表示设置音量的大小的操作不成功
	// return true 表示设置音量的大小操作成功

	long device;
	bool rc = false;
	MIXERCONTROL volCtrl;
	HMIXER hmixer;
	switch(dev){
	case WAVEOUT:
		device = MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT; 
		break;
	case SYNTHESIZER:
		device = MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER;
		break;
	case MICROPHONE:
		// device = MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC; 
		device = MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE;
		// device = MIXERLINE_COMPONENTTYPE_SRC_LINE;
		// device = MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC;
		break;

	default:
		device=MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
	}

	if(mixerOpen(&hmixer, 0, 0, 0, 0)) return 0;

	if (GetVolumeControl(hmixer, device, MIXERCONTROL_CONTROLTYPE_VOLUME, &volCtrl)){
		vol = vol*volCtrl.Bounds.lMaximum/m_VolRange;
		if(SetVolumeValue(hmixer, &volCtrl, vol))
			rc = true;
	}
	mixerClose(hmixer);
	return rc;
}

//设置设备静音
bool Mixer::SetMute(MixerDeive dev, bool vol){
	// dev = 0, 1, 2 分别表示主音量， MIDI, LINE IN
	// vol = 0, 1 分别表示取消静音，设置静音
	// return false 表示取消或设置静音操作不成功
	// return true 表示取消或设置静音操作成功

	long device;
	bool rc = false;
	MIXERCONTROL volCtrl;
	HMIXER hmixer;
	switch(dev){
	case WAVEOUT:
		device = MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT; 
		break;
	case SYNTHESIZER:
		device = MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER;
		break;
	case MICROPHONE:
		// device = MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC; 
		device = MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE;
		// device = MIXERLINE_COMPONENTTYPE_SRC_LINE;
		// device = MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC;
		break;

	default:
		device=MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
	}

	unsigned long err;
	err = mixerOpen(&hmixer, 0, 0, 0, 0);
	if (err){
		cout << "ERROR: Can't open Mixer Device!--" << err <<endl;
	}else{
		if(GetVolumeControl(hmixer, device, MIXERCONTROL_CONTROLTYPE_MUTE, &volCtrl)){
			cout << "SetMute : " << "Get Volume Control !" <<endl;
			if (SetMuteValue(hmixer, &volCtrl, (bool)vol)){
				cout << "SetMute : " << "Set Mute Value " << vol <<endl;
				rc = true;
			}
		}
		mixerClose(hmixer);
	}
	
	return rc;
}

bool Mixer::GetMute(MixerDeive dev){
	// dev = 0, 1, 2 分别表示主音量， MIDI, LINE IN
	// return false 表示没有静音
	// return true 表示静音
	long device;
	bool rc = false;
	MIXERCONTROL volCtrl;
	HMIXER hmixer;

	switch(dev){
		case WAVEOUT:
		device = MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT; 
		break;
	case SYNTHESIZER:
		device = MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER;
		break;
	case MICROPHONE:
		// device = MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC; 
		// device = MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE;
		// device = MIXERLINE_COMPONENTTYPE_SRC_LINE;
		// device = MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC;
		device = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;
		break;

	default:
		device=MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
	}
	if(mixerOpen(&hmixer, 0, 0, 0, 0)) return 0; 

	if (GetVolumeControl(hmixer, device, MIXERCONTROL_CONTROLTYPE_MUTE, &volCtrl)){
		rc = GetMuteValue(hmixer, &volCtrl);
	}
	mixerClose(hmixer);
	
	return rc;
}

bool Mixer::GetMixerNumDevs(){
	MIXERCAPS mixcaps;
	unsigned long iNumDevs; 
	int i;

	//获取系统中混音器设备个数
	iNumDevs = mixerGetNumDevs();

	//遍历所有的混音器并显示他们的ID和名称
	for(i = 0; i < iNumDevs; ++i){
		//获取下一个混音器设备的信息
		if (!mixerGetDevCaps(i, &mixcaps, sizeof(MIXERCAPS))){
			//显示ID和名称
			printf("Device ID #%u: %s\n", i, mixcaps.szPname);
		}
	}
	return true;
}