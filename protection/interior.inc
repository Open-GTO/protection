/*

	About: interior protection
	Author: ziggi

*/

#if !defined _protection_included
	#error "Please include protection before interior"
#endif

#if !defined _getspawninfo_included
	#error "Please include getspawninfo before interior"
#endif

#if defined _protection_interior_included
	#endinput
#endif

#define _protection_interior_included
#pragma library protection_interior

/*
	Vars
*/

static
	Float:gPlayerInterior[MAX_PLAYERS char];


/*
	OnPlayerInteriorChange
*/

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	new bool:is_enabled = Protection_GetParamInt(PROTECTION_INTERIOR, PROTECTION_ENABLED) != 0;
	
	if (is_enabled && gPlayerInterior{playerid} != newinteriorid) {
		SetPlayerInterior(playerid, _:gPlayerInterior{playerid});
		Protection_GivePlayerWarning(playerid, PROTECTION_INTERIOR, "%d != %d", gPlayerInterior{playerid}, newinteriorid);
	}

	#if defined Interior_OnPlayerInteriorChange
		return Interior_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerInteriorChange
	#undef OnPlayerInteriorChange
#else
	#define _ALS_OnPlayerInteriorChange
#endif
 
#define OnPlayerInteriorChange Interior_OnPlayerInteriorChange
#if defined Interior_OnPlayerInteriorChange
	forward Interior_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);
#endif

/*
	SetPlayerInterior
*/

stock Interior_SetPlayerInterior(playerid, interior)
{
	gPlayerInterior{playerid} = interior;
	return SetPlayerInterior(playerid, interior);
}
#if defined _ALS_SetPlayerInterior
	#undef SetPlayerInterior
#else
	#define _ALS_SetPlayerInterior
#endif
 
#define SetPlayerInterior Interior_SetPlayerInterior

/*
	DisableInteriorEnterExits
*/

stock Interior_DisableInteriorEE()
{
	Protection_SetParamInt(PROTECTION_INTERIOR, PROTECTION_ENABLED, 1);
	return DisableInteriorEnterExits();
}
#if defined _ALS_DisableInteriorEnterExits
	#undef DisableInteriorEnterExits
#else
	#define _ALS_DisableInteriorEnterExits
#endif
 
#define DisableInteriorEnterExits Interior_DisableInteriorEE
