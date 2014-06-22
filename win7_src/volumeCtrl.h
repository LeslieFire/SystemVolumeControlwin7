#pragma once

#include "stdafx.h"
#include <Windows.h>
#include <conio.h>
#include <commctrl.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

class volumeCtrlwin7{
public:
	volumeCtrlwin7();
	~volumeCtrlwin7();

	void setMute(bool bMute);
	void setVolume(float vol);

	BOOL getMute();
	float getVolume();

	const volumeCtrlwin7& operator++();
	const volumeCtrlwin7& operator--();

	void getVolumeRange();

private:
	bool validVol(float vol);

	IAudioEndpointVolume	*m_pEndptVolCtrl;
	IMMDeviceEnumerator		*m_pEnumerator;
	IMMDevice				*m_pDevice;

	HRESULT hr;
	float					m_vMin;
	float					m_vMax;
	float					m_vInc;

	float					m_volRange;
	float					m_currentVol;
};