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

/*
	Vars
*/

static
	gPlayerInterior[MAX_PLAYERS char],
	bool:gBlockSpawnReset[MAX_PLAYERS char];

/*
	OnPlayerConnect
*/

public OnPlayerConnect(playerid)
{
	gPlayerInterior{playerid} = 0;
	gBlockSpawnReset{playerid} = false;

	#if defined PInterio_OnPlayerConnect
		return PInterio_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect PInterio_OnPlayerConnect
#if defined PInterio_OnPlayerConnect
	forward PInterio_OnPlayerConnect(playerid);
#endif

/*
	OnPlayerSpawn
*/

public OnPlayerSpawn(playerid)
{
	if (gBlockSpawnReset{playerid}) {
		gBlockSpawnReset{playerid} = false;
	} else {
		gPlayerInterior{playerid} = 0;
	}

	#if defined PInterio_OnPlayerSpawn
		return PInterio_OnPlayerSpawn(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
	#define _ALS_OnPlayerSpawn
#endif

#define OnPlayerSpawn PInterio_OnPlayerSpawn
#if defined PInterio_OnPlayerSpawn
	forward PInterio_OnPlayerSpawn(playerid);
#endif

/*
	OnPlayerInteriorChange
*/

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	new bool:is_enabled = Protection_GetParamInt(PROTECTION_INTERIOR, PROTECTION_ENABLED) != 0;

	if (is_enabled && gPlayerInterior{playerid} != newinteriorid) {
		if (Protection_GetParamInt(PROTECTION_INTERIOR, PROTECTION_COUNTERMEASURE) && Protection_GetPlayerParamInt(playerid, PROTECTION_INTERIOR, PROTECTION_PLAYER_COUNTERMEASURE)) {
			SetPlayerInterior(playerid, gPlayerInterior{playerid});
		}

		Protection_GivePlayerWarning(playerid, PROTECTION_INTERIOR, "%d != %d", gPlayerInterior{playerid}, newinteriorid);
	}

	#if defined PInterio_OnPlayerInteriorChange
		return PInterio_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerInteriorChange
	#undef OnPlayerInteriorChange
#else
	#define _ALS_OnPlayerInteriorChange
#endif

#define OnPlayerInteriorChange PInterio_OnPlayerInteriorChange
#if defined PInterio_OnPlayerInteriorChange
	forward PInterio_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);
#endif

/*
	SetPlayerInterior
*/

stock PInterior_SetPlayerInterior(playerid, interior)
{
	if (!Protection_GetPlayerInfoInt(playerid, PROTECTION_PLAYER_SPAWNED)) {
		gBlockSpawnReset{playerid} = true;
	}

	gPlayerInterior{playerid} = interior;
	return SetPlayerInterior(playerid, interior);
}
#if defined _ALS_SetPlayerInterior
	#undef SetPlayerInterior
#else
	#define _ALS_SetPlayerInterior
#endif

#define SetPlayerInterior PInterior_SetPlayerInterior

/*
	DisableInteriorEnterExits
*/

stock PInterior_DisableInteriorEntExt()
{
	Protection_SetParamInt(PROTECTION_INTERIOR, PROTECTION_ENABLED, 1);
	return DisableInteriorEnterExits();
}
#if defined _ALS_DisableInteriorEnterExits
	#undef DisableInteriorEnterExits
#else
	#define _ALS_DisableInteriorEnterExits
#endif

#define DisableInteriorEnterExits PInterior_DisableInteriorEntExt
