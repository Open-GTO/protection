/*

	About: pause system
	Author: ziggi

*/

#if !defined _protection_included
	#error "Please include protection before pause"
#endif

#if defined _protection_pause_included
	#endinput
#endif

#define _protection_pause_included

/*
	Defines
*/

#if !defined PROTECTION_PAUSE_DELAY
	#define PROTECTION_PAUSE_DELAY 2000
#endif

/*
	Vars
*/

static
	gLastTick[MAX_PLAYERS];

/*
	OnPlayerUpdate
*/

public OnPlayerUpdate(playerid)
{
	gLastTick[playerid] = GetTickCount();

	#if defined PPause_OnPlayerUpdate
		return PPause_OnPlayerUpdate(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerUpdate
	#undef OnPlayerUpdate
#else
	#define _ALS_OnPlayerUpdate
#endif

#define OnPlayerUpdate PPause_OnPlayerUpdate
#if defined PPause_OnPlayerUpdate
	forward PPause_OnPlayerUpdate(playerid);
#endif

/*
	Functions
*/

stock Protection_GetPlayerLastUpdate(playerid)
{
	return gLastTick[playerid];
}

stock Protection_IsPlayerPaused(playerid)
{
	return Protection_GetTickDiff(GetTickCount(), gLastTick[playerid]) > PROTECTION_PAUSE_DELAY;
}
