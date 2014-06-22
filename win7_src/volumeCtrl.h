/*
 *  volumeCtrl.h
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
