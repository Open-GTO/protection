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

/*
	Defines
*/

#if !defined PROTECTION_TIMER_TIME
	#define PROTECTION_TIMER_TIME 300
#endif

#define PROTECTION_MAX_STRING 32
#define PROTECTION_MAX_IP     14

/*
	Forward
*/

forward Protection_Timer();
forward Float:Protection_GetParamFloat(Protection:protection, ProtectionParams:param);
forward Float:Protection_GetPlayerParamFloat(playerid, Protection:protection, ProtectionPlayerParams:param);
forward Float:Protection_GetPlayerInfoFloat(playerid, ProtectionPlayerInfo:param);

forward OnPlayerGetProtectionWarning(playerid, Protection:protection, bool:warn_reached, message[]);

/*
	Enums
*/

enum Protection {
	PROTECTION_ALL,
	PROTECTION_TELEPORT,
	PROTECTION_INTERIOR,
	PROTECTION_SPECIALACTION,
	PROTECTION_RCON,
	PROTECTION_PING,
	PROTECTION_HEALTH,
	PROTECTION_ARMOUR,
	PROTECTION_VEHICLETELEPORT,
	PROTECTION_WEAPON,
	PROTECTION_MONEY,
	PROTECTION_KILLFLOOD,
	PROTECTION_WEAPONDAMAGE,
	PROTECTION_RAPIDFIRE,
	PROTECTION_BADSHOT,
	PROTECTION_PICKUPPICK,
	PROTECTION_VEHICLECOLOR,
	PROTECTION_CAMERA,
}

enum ProtectionParams {
	PROTECTION_NAME[PROTECTION_MAX_STRING],
	PROTECTION_FUNCTION[PROTECTION_MAX_STRING],
	bool:PROTECTION_ENABLED,
	PROTECTION_CHECK_DELAY,
	PROTECTION_LAST_TICKCOUNT,
	PROTECTION_SPAWN_DELAY,
	PROTECTION_MAX_WARNINGS,
	Float:PROTECTION_MAX_DISTANCE,
	Float:PROTECTION_MAX_VEHICLE_DISTANCE,
	PROTECTION_MAX_PING,
}

enum ProtectionPlayerParams {
	PROTECTION_PLAYER_DISABLED,
	PROTECTION_PLAYER_WARNING_COUNT,
}

enum ProtectionPlayerInfo {
	bool:PROTECTION_PLAYER_SPAWNED,
	PROTECTION_PLAYER_IP[PROTECTION_MAX_IP + 1],
	PROTECTION_PLAYER_PING,
	PROTECTION_PLAYER_SPAWN_TICK,
}

/*
	Includes
*/

#include "protection\protection\teleport"
#include "protection\protection\interior"
#include "protection\protection\specialaction"
#include "protection\protection\rcon"
#include "protection\protection\ping"
#include "protection\protection\health"
#include "protection\protection\armour"
#include "protection\protection\vehicleteleport"
#include "protection\protection\weapon"
#include "protection\protection\money"
#include "protection\protection\killflood"
#include "protection\protection\rapidfire"
#include "protection\protection\badshot"
#include "protection\protection\pickuppick"
#include "protection\protection\vehiclecolor"
#include "protection\protection\camera"

/*
	Vars
*/

static
	gParam[Protection][ProtectionParams],
	gPlayerParam[MAX_PLAYERS][Protection][ProtectionPlayerParams],
	gPlayerInfo[MAX_PLAYERS][ProtectionPlayerInfo];

static const
	NULL_gPlayerParam[Protection][ProtectionPlayerParams],
	NULL_gPlayerInfo[ProtectionPlayerInfo];

/*
	OnGameModeInit
*/

public OnGameModeInit()
{
	Protection_SetParamInt(PROTECTION_ALL, PROTECTION_CHECK_DELAY, 1000);
	Protection_SetParamInt(PROTECTION_ALL, PROTECTION_MAX_WARNINGS, 3);
	Protection_SetParamInt(PROTECTION_ALL, PROTECTION_SPAWN_DELAY, 600);

	// teleport
	Protection_SetParamInt(PROTECTION_TELEPORT, PROTECTION_ENABLED, 1);
	Protection_SetParamString(PROTECTION_TELEPORT, PROTECTION_NAME, "Teleport");
	Protection_SetParamString(PROTECTION_TELEPORT, PROTECTION_FUNCTION, "Protection_Teleport");
	Protection_SetParamFloat(PROTECTION_TELEPORT, PROTECTION_MAX_VEHICLE_DISTANCE, 140.0);
	Protection_SetParamFloat(PROTECTION_TELEPORT, PROTECTION_MAX_DISTANCE, 80.0);

	// interior (disabled, but is automatically activated when used DisableInteriorEnterExits)
	Protection_SetParamInt(PROTECTION_INTERIOR, PROTECTION_ENABLED, 0);
	Protection_SetParamString(PROTECTION_INTERIOR, PROTECTION_NAME, "Interior");

	// special action
	Protection_SetParamInt(PROTECTION_SPECIALACTION, PROTECTION_ENABLED, 1);
	Protection_SetParamString(PROTECTION_SPECIALACTION, PROTECTION_NAME, "Special action");
	Protection_SetParamString(PROTECTION_SPECIALACTION, PROTECTION_FUNCTION, "Protection_SpecialAction");

	// rcon
	Protection_SetParamInt(PROTECTION_RCON, PROTECTION_ENABLED, 1);
	Protection_SetParamString(PROTECTION_RCON, PROTECTION_NAME, "Rcon");

	// ping
	Protection_SetParamInt(PROTECTION_PING, PROTECTION_ENABLED, 1);
	Protection_SetParamString(PROTECTION_PING, PROTECTION_NAME, "Ping");
	Protection_SetParamString(PROTECTION_PING, PROTECTION_FUNCTION, "Protection_Ping");
	Protection_SetParamInt(PROTECTION_PING, PROTECTION_MAX_PING, 500);

	// health
	Protection_SetParamInt(PROTECTION_HEALTH, PROTECTION_ENABLED, 1);
	Protection_SetParamString(PROTECTION_HEALTH, PROTECTION_NAME, "Health");
	Protection_SetParamString(PROTECTION_HEALTH, PROTECTION_FUNCTION, "Protection_Health");
	Protection_SetParamInt(PROTECTION_HEALTH, PROTECTION_SPAWN_DELAY, 2000);

	// armour
	Protection_SetParamInt(PROTECTION_ARMOUR, PROTECTION_ENABLED, 1);
	Protection_SetParamString(PROTECTION_ARMOUR, PROTECTION_NAME, "Armour");
	Protection_SetParamString(PROTECTION_ARMOUR, PROTECTION_FUNCTION, "Protection_Armour");

	// vehicleteleport
	Protection_SetParamInt(PROTECTION_VEHICLETELEPORT, PROTECTION_ENABLED, 1);
	Protection_SetParamString(PROTECTION_VEHICLETELEPORT, PROTECTION_NAME, "Vehicle Teleport");
	Protection_SetParamFloat(PROTECTION_VEHICLETELEPORT, PROTECTION_MAX_DISTANCE, 20.0);

	// weapon
	Protection_SetParamInt(PROTECTION_WEAPON, PROTECTION_ENABLED, 1);
	Protection_SetParamString(PROTECTION_WEAPON, PROTECTION_NAME, "Weapon");
	Protection_SetParamString(PROTECTION_WEAPON, PROTECTION_FUNCTION, "Protection_Weapon");

	// money
	Protection_SetParamInt(PROTECTION_MONEY, PROTECTION_ENABLED, 1);
	Protection_SetParamString(PROTECTION_MONEY, PROTECTION_NAME, "Money");
	Protection_SetParamString(PROTECTION_MONEY, PROTECTION_FUNCTION, "Protection_Money");
	Protection_SetParamInt(PROTECTION_MONEY, PROTECTION_SPAWN_DELAY, 2000);

	// killflood
	Protection_SetParamInt(PROTECTION_KILLFLOOD, PROTECTION_ENABLED, 1);
	Protection_SetParamString(PROTECTION_KILLFLOOD, PROTECTION_NAME, "Kill Flood");

	// weapondamage
	Protection_SetParamInt(PROTECTION_WEAPONDAMAGE, PROTECTION_ENABLED, 1);
	Protection_SetParamString(PROTECTION_WEAPONDAMAGE, PROTECTION_NAME, "Weapon Damage");

	// rapidfire
	Protection_SetParamInt(PROTECTION_RAPIDFIRE, PROTECTION_ENABLED, 1);
	Protection_SetParamString(PROTECTION_RAPIDFIRE, PROTECTION_NAME, "Rapid Fire");

	// badshot
	Protection_SetParamInt(PROTECTION_BADSHOT, PROTECTION_ENABLED, 1);
	Protection_SetParamString(PROTECTION_BADSHOT, PROTECTION_NAME, "Bad Shot");

	// pickuppick
	Protection_SetParamInt(PROTECTION_PICKUPPICK, PROTECTION_ENABLED, 1);
	Protection_SetParamString(PROTECTION_PICKUPPICK, PROTECTION_NAME, "Range Pickup Pick");
	Protection_SetParamFloat(PROTECTION_PICKUPPICK, PROTECTION_MAX_DISTANCE, 10.0);

	// vehiclecolor
	Protection_SetParamInt(PROTECTION_VEHICLECOLOR, PROTECTION_ENABLED, 1);
	Protection_SetParamString(PROTECTION_VEHICLECOLOR, PROTECTION_NAME, "Vehicle Color");

	// camera
	Protection_SetParamInt(PROTECTION_CAMERA, PROTECTION_ENABLED, 1);
	Protection_SetParamString(PROTECTION_CAMERA, PROTECTION_NAME, "Camera");
	Protection_SetParamString(PROTECTION_CAMERA, PROTECTION_FUNCTION, "Protection_Camera");
	Protection_SetParamFloat(PROTECTION_CAMERA, PROTECTION_MAX_DISTANCE, 200.0);
	Protection_SetParamInt(PROTECTION_CAMERA, PROTECTION_SPAWN_DELAY, 2000);

	// timer
	SetTimer("Protection_Timer", PROTECTION_TIMER_TIME, 1);

	// compiler crash fix
#if __Pawn != 0x030A
	new crash_fix[1];
	format(crash_fix, 1, "");
#endif

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
	// reset arrays data
	gPlayerParam[playerid] = NULL_gPlayerParam;
	gPlayerInfo[playerid] = NULL_gPlayerInfo;

	// store connect data
	new
		player_ip[PROTECTION_MAX_IP + 1];

	GetPlayerIp(playerid, player_ip, sizeof(player_ip));

	Protection_SetPlayerInfoString(playerid, PROTECTION_PLAYER_IP, player_ip);

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
			Protection_SetPlayerInfoInt(playerid, PROTECTION_PLAYER_SPAWN_TICK, GetTickCount());
		}
		case PLAYER_STATE_WASTED, PLAYER_STATE_SPECTATING: {
			Protection_SetPlayerInfoInt(playerid, PROTECTION_PLAYER_SPAWNED, 0);
		}
	}

	// fix after killflood cheat
	if (oldstate == PLAYER_STATE_WASTED) {
		switch (newstate) {
			case PLAYER_STATE_ONFOOT, PLAYER_STATE_DRIVER, PLAYER_STATE_PASSENGER: {
				Protection_SetPlayerInfoInt(playerid, PROTECTION_PLAYER_SPAWNED, 1);
				Protection_SetPlayerInfoInt(playerid, PROTECTION_PLAYER_SPAWN_TICK, GetTickCount());
			}
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
		player_ping,
		Protection:current_prot,
		spawn_tick,
		current_tick,
		last_tick,
		function[PROTECTION_MAX_STRING];

	current_tick = GetTickCount();

	foreach (new playerid : Player) {
		player_ping = GetPlayerPing(playerid);
		Protection_SetPlayerInfoInt(playerid, PROTECTION_PLAYER_PING, player_ping);

		spawn_tick = Protection_GetPlayerInfoInt(playerid, PROTECTION_PLAYER_SPAWN_TICK);

		// spawned check
		if (!Protection_GetPlayerInfoInt(playerid, PROTECTION_PLAYER_SPAWNED)) {
			continue;
		}

		for (new i = 1; i < sizeof(gParam); i++) {
			current_prot = Protection:i;

			// spawn delay
			if (Protection_GetTickDiff(current_tick, spawn_tick) < player_ping + Protection_GetParamInt(current_prot, PROTECTION_SPAWN_DELAY)) {
				continue;
			}

			// enabled check
			if (!Protection_GetParamInt(current_prot, PROTECTION_ENABLED)) {
				continue;
			}

			// player enabled check
			if (Protection_GetPlayerParamInt(playerid, current_prot, PROTECTION_PLAYER_DISABLED)) {
				continue;
			}

			// exists check
			Protection_GetParamString(current_prot, PROTECTION_FUNCTION, function);

			if (funcidx(function) == -1) {
				continue;
			}

			// delay check
			last_tick = Protection_GetParamInt(current_prot, PROTECTION_LAST_TICKCOUNT);

			if (Protection_GetTickDiff(current_tick, last_tick) < player_ping + Protection_GetParamInt(current_prot, PROTECTION_CHECK_DELAY)) {
				continue;
			}

			Protection_SetParamInt(current_prot, PROTECTION_LAST_TICKCOUNT, current_tick);

			// execute
			CallRemoteFunction(function, "i", playerid);
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

	warn_count = Protection_GetPlayerParamInt(playerid, protection, PROTECTION_PLAYER_WARNING_COUNT);
	warn_count++;

	Protection_SetPlayerParamInt(playerid, protection, PROTECTION_PLAYER_WARNING_COUNT, warn_count);

	// format
	#define BYTES_PER_CELL 4
	static const STATIC_ARGS = 3;

	new n = (numargs() - STATIC_ARGS) * BYTES_PER_CELL;

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
			arg_end -= BYTES_PER_CELL;
			#emit LOAD.S.pri	arg_end
		} while (arg_end > arg_start);

		#emit PUSH.S			fstring
		#emit PUSH.C			144
		#emit PUSH.ADR			message

		n += BYTES_PER_CELL * 3;
		#emit PUSH.S			n
		#emit SYSREQ.C			format

		n += BYTES_PER_CELL;
		#emit LCTRL				4
		#emit LOAD.S.alt		n
		#emit ADD
		#emit SCTRL				4

		CallRemoteFunction("OnPlayerGetProtectionWarning", "iiis", playerid, _:protection, warn_count >= warn_max, message);
	} else {
		if (fstring[0] == '\0') {
			fstring[0] = '\1';
		}

		CallRemoteFunction("OnPlayerGetProtectionWarning", "iiis", playerid, _:protection, warn_count >= warn_max, fstring);
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

stock Protection_SetPlayerParamString(playerid, Protection:protection, ProtectionPlayerParams:param, value[])
{
	if (protection == PROTECTION_ALL) {
		for (new i = 0; i < sizeof(gPlayerParam[]); i++) {
			strmid(gPlayerParam[playerid][Protection:i][param], value, 0, strlen(value), PROTECTION_MAX_STRING);
		}
	} else {
		strmid(gPlayerParam[playerid][protection][param], value, 0, strlen(value), PROTECTION_MAX_STRING);
	}
}

stock Protection_GetPlayerParamString(playerid, Protection:protection, ProtectionPlayerParams:param, value[], size = sizeof(value))
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

stock Protection_SetPlayerInfoFloat(playerid, ProtectionPlayerInfo:param, Float:value)
{
	gPlayerInfo[playerid][param] = value;
}

stock Float:Protection_GetPlayerInfoFloat(playerid, ProtectionPlayerInfo:param)
{
	return gPlayerInfo[playerid][param];
}

/*
	Player info string params
*/

stock Protection_SetPlayerInfoString(playerid, ProtectionPlayerInfo:param, value[])
{
	strmid(gPlayerInfo[playerid][param], value, 0, strlen(value), PROTECTION_MAX_STRING);
}

stock Protection_GetPlayerInfoString(playerid, ProtectionPlayerInfo:param, value[], size = sizeof(value))
{
	strmid(value, gPlayerInfo[playerid][param], 0, strlen(gPlayerInfo[playerid][param]), size);
}

/*
	Safe tick different
*/

stock Protection_GetTickDiff(newtick, oldtick)
{
	if (oldtick < 0 && newtick >= 0) {
		return newtick - oldtick;
	} else if (oldtick >= 0 && newtick < 0 || oldtick > newtick) {
		return (cellmax - oldtick + 1) - (cellmin - newtick);
	}
	return newtick - oldtick;
}
