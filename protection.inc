/*

	About: cheat protection stuff
	Author: ziggi

*/

#if !defined _samp_included
	#error "Please include a_samp or a_npc before protection"
#endif

#if !defined _FOREACH_LOCAL_VERSION
	#error "Please include foreach before protection"
#endif

#if defined _protection_included
	#endinput
#endif

#define _protection_included
#pragma library protection

/*
	Defines
*/

#if !defined PROTECTION_TIMER_TIME
	#define PROTECTION_TIMER_TIME 300
#endif

#define PROTECTION_MAX_STRING 32

/*
	Forward	
*/

forward Protection_Timer();
forward Float:Protection_GetParamFloat(Protection:protection, ProtectionParams:param);
forward Float:Protection_GetPlayerParamFloat(playerid, Protection:protection, ProtectionPlayerParams:param);
forward Float:Protection_GetPlayerInfoFloat(playerid, ProtectionPlayerParams:param);

forward OnPlayerGetProtectionWarning(playerid, Protection:protection, bool:warn_reached, message[]);

/*
	Enums
*/

enum Protection {
	PROTECTION_ALL,
	PROTECTION_TELEPORT,
}

enum ProtectionParams {
	PROTECTION_NAME[PROTECTION_MAX_STRING],
	PROTECTION_FUNCTION[PROTECTION_MAX_STRING],
	bool:PROTECTION_ENABLED,
	PROTECTION_CHECK_DELAY,
	PROTECTION_LAST_TICKCOUNT,
	PROTECTION_MAX_WARNINGS,
	Float:PROTECTION_MAX_DISTANCE,
	Float:PROTECTION_MAX_VEHICLE_DISTANCE,
}

enum ProtectionPlayerParams {
	PROTECTION_WARNING_COUNT,
}

enum ProtectionPlayerInfo {
	bool:PROTECTION_PLAYER_SPAWNED,
}

/*
	Includes
*/

#include "protection/teleport"

/*
	Vars
*/

static
	gParam[Protection][ProtectionParams],
	gPlayerParam[MAX_PLAYERS][Protection][ProtectionPlayerParams],
	gPlayerInfo[MAX_PLAYERS][ProtectionPlayerInfo];

static const
	NULL_gPlayerParam[Protection][ProtectionPlayerParams];

/*
	OnGameModeInit
*/

public OnGameModeInit()
{
	Protection_SetParamInt(PROTECTION_ALL, PROTECTION_ENABLED, 1);
	Protection_SetParamInt(PROTECTION_ALL, PROTECTION_CHECK_DELAY, 1000);
	Protection_SetParamInt(PROTECTION_ALL, PROTECTION_MAX_WARNINGS, 3);

	// teleport
	Protection_SetParamFloat(PROTECTION_TELEPORT, PROTECTION_MAX_VEHICLE_DISTANCE, 140.0);
	Protection_SetParamFloat(PROTECTION_TELEPORT, PROTECTION_MAX_DISTANCE, 80.0);
	Protection_SetParamString(PROTECTION_TELEPORT, PROTECTION_NAME, "Teleport");
	Protection_SetParamString(PROTECTION_TELEPORT, PROTECTION_FUNCTION, "Protection_Teleport");

	// timer
	SetTimer("Protection_Timer", PROTECTION_TIMER_TIME, 1);

	#if defined Protection_OnGameModeInit
		return Protection_OnGameModeInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
 
#define OnGameModeInit Protection_OnGameModeInit
#if defined Protection_OnGameModeInit
	forward Protection_OnGameModeInit();
#endif

/*
	OnPlayerConnect
*/

public OnPlayerConnect(playerid)
{
	gPlayerParam[playerid] = NULL_gPlayerParam;

	#if defined Protection_OnPlayerConnect
		return Protection_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
 
#define OnPlayerConnect Protection_OnPlayerConnect
#if defined Protection_OnPlayerConnect
	forward Protection_OnPlayerConnect(playerid);
#endif

/*
	OnPlayerStateChange
*/

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	switch (newstate) {
		case PLAYER_STATE_SPAWNED: {
			Protection_SetPlayerInfoInt(playerid, PROTECTION_PLAYER_SPAWNED, 1);
		}
		case PLAYER_STATE_WASTED, PLAYER_STATE_SPECTATING: {
			Protection_SetPlayerInfoInt(playerid, PROTECTION_PLAYER_SPAWNED, 0);
		}
	}

	#if defined Protection_OnPlayerStateChange
		return Protection_OnPlayerStateChange(playerid, newstate, oldstate);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerStateChange
	#undef OnPlayerStateChange
#else
	#define _ALS_OnPlayerStateChange
#endif
 
#define OnPlayerStateChange Protection_OnPlayerStateChange
#if defined Protection_OnPlayerStateChange
	forward Protection_OnPlayerStateChange(playerid, newstate, oldstate);
#endif

/*
	Timer
*/

public Protection_Timer()
{
	static
		Protection:current_prot,
		current_tick,
		last_tick,
		function[PROTECTION_MAX_STRING];

	current_tick = GetTickCount();

	foreach (new playerid : Player) {
		for (new i = 1; i < sizeof(gParam); i++) {
			current_prot = Protection:i;
			
			// enabled check
			if (!Protection_GetParamInt(current_prot, PROTECTION_ENABLED)) {
				continue;
			}

			// exists check
			Protection_GetParamString(current_prot, PROTECTION_FUNCTION, function);

			if (funcidx(function) == -1) {
				continue;
			}

			// delay check
			last_tick = Protection_GetParamInt(current_prot, PROTECTION_LAST_TICKCOUNT);

			if (current_tick - last_tick < Protection_GetParamInt(current_prot, PROTECTION_CHECK_DELAY)) {
				continue;
			}

			Protection_SetParamInt(current_prot, PROTECTION_LAST_TICKCOUNT, current_tick);

			// execute
			CallLocalFunction(function, "i", playerid);
		}
	}

	return 1;
}

/*
	Protection report
*/

stock Protection_GivePlayerWarning(playerid, Protection:protection, fstring[] = "", {Float, _}:...)
{
	static
		warn_max,
		warn_count;

	warn_max = Protection_GetParamInt(protection, PROTECTION_MAX_WARNINGS);

	warn_count = Protection_GetPlayerParamInt(playerid, protection, PROTECTION_WARNING_COUNT);
	warn_count++;

	Protection_SetPlayerParamInt(playerid, protection, PROTECTION_WARNING_COUNT, warn_count);

	// format
	if (funcidx("OnPlayerGetProtectionWarning") == -1) {
		return 0;
	}

	static const STATIC_ARGS = 3;
	new n = (numargs() - STATIC_ARGS) * 4;
	if (n) {
		new message[144], arg_start, arg_end;
		#emit CONST.alt			fstring
		#emit LCTRL				5
		#emit ADD
		#emit STOR.S.pri		arg_start

		#emit LOAD.S.alt		n
		#emit ADD
		#emit STOR.S.pri		arg_end
		do {
			#emit LOAD.I
			#emit PUSH.pri
			arg_end -= 4;
			#emit LOAD.S.pri	arg_end
		} while (arg_end > arg_start);

		#emit PUSH.S			fstring
		#emit PUSH.C			144
		#emit PUSH.ADR			message

		n += 4 * 3;
		#emit PUSH.S			n
		#emit SYSREQ.C			format

		n += 4;
		#emit LCTRL				4
		#emit LOAD.S.alt		n
		#emit ADD
		#emit SCTRL				4

		CallLocalFunction("OnPlayerGetProtectionWarning", "iiis", playerid, _:protection, warn_count >= warn_max, message);
	} else {
		if (fstring[0] == '\0') {
			fstring[0] = '\1';
		}

		CallLocalFunction("OnPlayerGetProtectionWarning", "iiis", playerid, _:protection, warn_count >= warn_max, fstring);
	}

	return 1;
}

/*
	Integer params
*/

stock Protection_SetParamInt(Protection:protection, ProtectionParams:param, value)
{
	if (protection == PROTECTION_ALL) {
		for (new i = 0; i < sizeof(gParam); i++) {
			gParam[Protection:i][param] = value;
		}
	} else {
		gParam[protection][param] = value;
	}
}

stock Protection_GetParamInt(Protection:protection, ProtectionParams:param)
{
	return gParam[protection][param];
}

/*
	Specific float params
*/

stock Protection_SetParamFloat(Protection:protection, ProtectionParams:param, Float:value)
{
	if (protection == PROTECTION_ALL) {
		for (new i = 0; i < sizeof(gParam); i++) {
			gParam[Protection:i][param] = _:value;
		}
	} else {
		gParam[protection][param] = _:value;
	}
}

stock Float:Protection_GetParamFloat(Protection:protection, ProtectionParams:param)
{
	return Float:gParam[protection][param];
}

/*
	String params
*/

stock Protection_SetParamString(Protection:protection, ProtectionParams:param, value[])
{
	if (protection == PROTECTION_ALL) {
		for (new i = 0; i < sizeof(gParam); i++) {
			strmid(gParam[Protection:i][param], value, 0, strlen(value), PROTECTION_MAX_STRING);
		}
	} else {
		strmid(gParam[protection][param], value, 0, strlen(value), PROTECTION_MAX_STRING);
	}
}

stock Protection_GetParamString(Protection:protection, ProtectionParams:param, value[], size = sizeof(value))
{
	strmid(value, gParam[protection][param], 0, strlen(gParam[protection][param]), size);
}

/*
	Player integer params
*/

stock Protection_SetPlayerParamInt(playerid, Protection:protection, ProtectionPlayerParams:param, value)
{
	if (protection == PROTECTION_ALL) {
		for (new i = 0; i < sizeof(gPlayerParam[]); i++) {
			gPlayerParam[playerid][Protection:i][param] = value;
		}
	} else {
		gPlayerParam[playerid][protection][param] = value;
	}
}

stock Protection_GetPlayerParamInt(playerid, Protection:protection, ProtectionPlayerParams:param)
{
	return gPlayerParam[playerid][protection][param];
}

/*
	Player float params
*/

stock Protection_SetPlayerParamFloat(playerid, Protection:protection, ProtectionPlayerParams:param, Float:value)
{
	if (protection == PROTECTION_ALL) {
		for (new i = 0; i < sizeof(gPlayerParam[]); i++) {
			gPlayerParam[playerid][Protection:i][param] = value;
		}
	} else {
		gPlayerParam[playerid][protection][param] = value;
	}
}

stock Float:Protection_GetPlayerParamFloat(playerid, Protection:protection, ProtectionPlayerParams:param)
{
	return gPlayerParam[playerid][protection][param];
}

/*
	Player string params
*/

stock Protection_SetPlayerParamString(playerid, Protection:protection, ProtectionParams:param, value[])
{
	if (protection == PROTECTION_ALL) {
		for (new i = 0; i < sizeof(gPlayerParam[]); i++) {
			strmid(gPlayerParam[playerid][Protection:i][param], value, 0, strlen(value), PROTECTION_MAX_STRING);
		}
	} else {
		strmid(gPlayerParam[playerid][protection][param], value, 0, strlen(value), PROTECTION_MAX_STRING);
	}
}

stock Protection_GetPlayerParamString(playerid, Protection:protection, ProtectionParams:param, value[], size = sizeof(value))
{
	strmid(value, gPlayerParam[playerid][protection][param], 0, strlen(gPlayerParam[playerid][protection][param]), size);
}

/*
	Player info integer params
*/

stock Protection_SetPlayerInfoInt(playerid, ProtectionPlayerInfo:param, value)
{
	gPlayerInfo[playerid][param] = value;
}

stock Protection_GetPlayerInfoInt(playerid, ProtectionPlayerInfo:param)
{
	return gPlayerInfo[playerid][param];
}

/*
	Player info float params
*/

stock Protection_SetPlayerInfoFloat(playerid, ProtectionPlayerParams:param, Float:value)
{
	gPlayerInfo[playerid][param] = value;
}

stock Float:Protection_GetPlayerInfoFloat(playerid, ProtectionPlayerParams:param)
{
	return gPlayerInfo[playerid][param];
}

/*
	Player info string params
*/

stock Protection_SetPlayerInfoString(playerid, ProtectionParams:param, value[])
{
	strmid(gPlayerInfo[playerid][param], value, 0, strlen(value), PROTECTION_MAX_STRING);
}

stock Protection_GetPlayerInfoString(playerid, ProtectionParams:param, value[], size = sizeof(value))
{
	strmid(value, gPlayerInfo[playerid][param], 0, strlen(gPlayerInfo[playerid][param]), size);
}
