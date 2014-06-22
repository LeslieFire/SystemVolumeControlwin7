/*
 *  volumeCtrl.cpp
 *
 *  Created by Leslie Yang on 06/22/14.
 *  Copyright 2014 https://github.com/LeslieFire All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the author nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 *  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 *  OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 *  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 *  OF THE POSSIBILITY OF SUCH DAMAGE.
 *s
 *  ************************************************************************************ */ 


#include "stdafx.h"
#include "volumeCtrl.h"

volumeCtrlwin7::volumeCtrlwin7(){
	hr = S_OK;
	m_volRange = 100.0f;
	const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
	const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);

	// Initialize COM component
	hr = CoInitialize(NULL);
	if(FAILED(hr)){
		printf("CoInitialize failed");
	}

	// Create instance for MMDevices
	hr = CoCreateInstance(CLSID_MMDeviceEnumerator,
		NULL,
		CLSCTX_ALL,
		IID_IMMDeviceEnumerator,
		(void**)&m_pEnumerator);
	if (FAILED(hr)){
		printf("CoCreateInstance failed");
	}

	// Get default audio endpoint
	hr = m_pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &m_pDevice);
	if (FAILED(hr)){
		printf("GetDefaultAudioEndpoint failed!");
	}

	// Activate devices
	hr = m_pDevice->Activate(__uuidof(IAudioEndpointVolume),
		CLSCTX_ALL,
		NULL,
		(void**)&m_pEndptVolCtrl);
	if (FAILED(hr)){
		printf("Activate failed!");
	}

	getVolumeRange();
	getVolume();
}

void volumeCtrlwin7::setMute(bool bMute){
	hr = m_pEndptVolCtrl->SetMute(bMute, &GUID_NULL);
	if(FAILED(hr)){
		printf("ERROR: failed to set Mute  #%d\n",hr);
	}
}

bool volumeCtrlwin7::validVol(float vol){
	return (vol >= 0.0f && vol <= 100.0f) ? true : false;
}

void volumeCtrlwin7::setVolume(float vol){
	if (validVol(vol)){
		m_currentVol = vol;
		vol /= m_volRange;
		hr = m_pEndptVolCtrl->SetMasterVolumeLevelScalar(vol, &GUID_NULL);
		
		printf("Success: set volume: %f\n", vol);
	}else{
		printf("ERROR: invalid volume value!\n");
	}
}

BOOL volumeCtrlwin7::getMute(){
	BOOL mute;
	m_pEndptVolCtrl->GetMute(&mute);
	return mute;
}

float volumeCtrlwin7::getVolume(){
	float vol;
	m_pEndptVolCtrl->GetMasterVolumeLevelScalar(&vol);
	//m_pEndptVolCtrl->GetChannelVolumeLevelScalar(0, &vol1);
	//m_pEndptVolCtrl->GetChannelVolumeLevelScalar(1, &vol2);
	
	m_currentVol = vol* m_volRange;
	printf("Current volume:  %f\n", m_currentVol);
	return m_currentVol;
}

void volumeCtrlwin7::getVolumeRange(){

	hr = m_pEndptVolCtrl->GetVolumeRange(&m_vMin, &m_vMax, &m_vInc);
	if (FAILED(hr)){
		printf("ERROR: failed to get volume range  #%d\n",hr);
	}else{
		printf("Max Volume : %f\nMin Volume : %f\nVolume interval : %f\n", m_vMax, m_vMin, m_vInc);
	}
}

volumeCtrlwin7::~volumeCtrlwin7(){
	CoUninitialize();
}

const volumeCtrlwin7 &volumeCtrlwin7::operator++(){
	m_currentVol += m_vInc*m_volRange;
	setVolume(m_currentVol);

	return *this;
}

const volumeCtrlwin7 &volumeCtrlwin7::operator--(){

	m_currentVol -= m_vInc*m_volRange;
	setVolume(m_currentVol);

	return *this;
}

