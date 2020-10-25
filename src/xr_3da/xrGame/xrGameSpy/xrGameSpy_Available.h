#pragma once
#include "xrGameSpy_MainDefs.h"

#include <GameSpy/src/GameSpy/nonport.h>
#include <GameSpy/src/GameSpy/common/gsAvailable.h>

extern "C"
{
//	EXPORT_FN_DECL(void,	GSIStartAvailableCheck, (const gsi_char * gamename));
	EXPORT_FN_DECL(void,	GSIStartAvailableCheck, ());
	EXPORT_FN_DECL(GSIACResult, GSIAvailableCheckThink, ());
	EXPORT_FN_DECL(void, msleep, (gsi_time msec));
}