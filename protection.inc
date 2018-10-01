/*

	About: cheat protection stuff
	Author: ziggi

*/

#if !defined _samp_included
	#error "Please include a_samp or a_npc before protection"
#endif

#if !defined foreach
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

#if !defined PROTECTION_USE_STREAMER
	#if defined Streamer_IncludeFileVersion
		#define PROTECTION_USE_STREAMER 1
	#else
		#define PROTECTION_USE_STREAMER 0
	#endif
#endif

#if !defined PROTECTION_CUSTOM_VENDING
	#define PROTECTION_CUSTOM_VENDING 1
#endif

#if !defined PROTECTION_MAX_AVAILABLE_IDS
	#define PROTECTION_MAX_AVAILABLE_IDS 100
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
	PROTECTION_CAMERAMODE,
	PROTECTION_FAKEKILL,
	PROTECTION_VEHICLETUNING,
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
	PROTECTION_AVAILABLE_IDS[PROTECTION_MAX_AVAILABLE_IDS],
}

enum ProtectionPlayerParams {
	PROTECTION_PLAYER_DISABLED,
	PROTECTION_PLAYER_WARNING_COUNT,
	PROTECTION_PLAYER_BLOCK_UPDATE,
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

#include "protection/pause"
#include "protection/teleport"
#include "protection/interior"
#include "protection/specialaction"
#include "protection/rcon"
#include "protection/ping"
#include "protection/health"
#include "protection/armour"
#include "protection/vehicleteleport"
#include "protection/weapon"
#include "protection/money"
#include "protection/killflood"
#include "protection/rapidfire"
#include "protection/badshot"
#include "protection/pickuppick"
#include "protection/vehiclecolor"
#include "protection/camera"
#include "protection/cameramode"
#include "protection/fakekill"
#include "protection/vehicletuning"

#if PROTECTION_CUSTOM_VENDING
	#include "protection/vending"
#endif

/*
	Vars
*/

static
	gParam[Protection][ProtectionParams],
	gPlayerParam[MAX_PLAYERS][Protection][ProtectionPlayerParams],
	gPlayerInfo[MAX_PLAYERS][ProtectionPlayerInfo];

static const
	NULL_gPlayerParam[ProtectionPlayerParams],
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
	Protection_SetParamFloat(PROTECTION_CAMERA, PROTECTION_MAX_DISTANCE, 80.0);
	Protection_SetParamInt(PROTECTION_CAMERA, PROTECTION_SPAWN_DELAY, 2000);

	// cameramode
	Protection_SetParamInt(PROTECTION_CAMERAMODE, PROTECTION_ENABLED, 1);
	Protection_SetParamString(PROTECTION_CAMERAMODE, PROTECTION_NAME, "Camera Mode");
	Protection_SetParamString(PROTECTION_CAMERAMODE, PROTECTION_FUNCTION, "Protection_CameraMode");
	Protection_SetParamArray(PROTECTION_CAMERAMODE, PROTECTION_AVAILABLE_IDS, {3, 4, 7, 8, 15, 16, 18, 22, 46, 51, 53, 55, 56, 57, 58, 59, 62, 63, 64, -1});

	// fakekill
	Protection_SetParamInt(PROTECTION_FAKEKILL, PROTECTION_ENABLED, 1);
	Protection_SetParamString(PROTECTION_FAKEKILL, PROTECTION_NAME, "Fake Kill");

	// vehicletuning
	Protection_SetParamInt(PROTECTION_VEHICLETUNING, PROTECTION_ENABLED, 1);
	Protection_SetParamString(PROTECTION_VEHICLETUNING, PROTECTION_NAME, "Vehicle Tuning");

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
	// reset arrays data
	for (new i = 0; i < sizeof(gParam); i++) {
		gPlayerParam[playerid][Protection:i] = NULL_gPlayerParam;
	}
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
	OnPlayerUpdate
*/

public OnPlayerUpdate(playerid)
{
	static
		i,
		bool:blocked;

	for (i = 1; i < sizeof(gParam); i++) {
		blocked = bool:Protection_GetPlayerParamInt(playerid, Protection:i, PROTECTION_PLAYER_BLOCK_UPDATE);
		if (blocked) {
			return 0;
		}
	}

	#if defined Protection_OnPlayerUpdate
		return Protection_OnPlayerUpdate(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerUpdate
	#undef OnPlayerUpdate
#else
	#define _ALS_OnPlayerUpdate
#endif

#define OnPlayerUpdate Protection_OnPlayerUpdate
#if defined Protection_OnPlayerUpdate
	forward Protection_OnPlayerUpdate(playerid);
#endif

/*
	OnPlayerWeaponShot
*/

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	static
		i,
		bool:blocked;

	for (i = 1; i < sizeof(gParam); i++) {
		blocked = bool:Protection_GetPlayerParamInt(playerid, Protection:i, PROTECTION_PLAYER_BLOCK_UPDATE);
		if (blocked) {
			return 0;
		}
	}

	#if defined Protection_OnPlayerWeaponShot
		return Protection_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerWeaponShot
	#undef OnPlayerWeaponShot
#else
	#define _ALS_OnPlayerWeaponShot
#endif

#define OnPlayerWeaponShot Protection_OnPlayerWeaponShot
#if defined Protection_OnPlayerWeaponShot
	forward Protection_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
#endif

/*
	OnUnoccupiedVehicleUpdate
*/

public OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat, Float:new_x, Float:new_y, Float:new_z, Float:vel_x, Float:vel_y, Float:vel_z)
{
	static
		i,
		bool:blocked;

	for (i = 1; i < sizeof(gParam); i++) {
		blocked = bool:Protection_GetPlayerParamInt(playerid, Protection:i, PROTECTION_PLAYER_BLOCK_UPDATE);
		if (blocked) {
			return 0;
		}
	}

	#if defined Prot_OnUnoccupiedVehicleUpdate
		return Prot_OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat, new_x, new_y, new_z, vel_x, vel_y, vel_z);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnUnoccupiedVehicleUpdate
	#undef OnUnoccupiedVehicleUpdate
#else
	#define _ALS_OnUnoccupiedVehicleUpdate
#endif

#define OnUnoccupiedVehicleUpdate Prot_OnUnoccupiedVehicleUpdate
#if defined Prot_OnUnoccupiedVehicleUpdate
	forward Prot_OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat, Float:new_x, Float:new_y, Float:new_z, Float:vel_x, Float:vel_y, Float:vel_z);
#endif

/*
	OnTrailerUpdate
*/

public OnTrailerUpdate(playerid, vehicleid)
{
	static
		i,
		bool:blocked;

	for (i = 1; i < sizeof(gParam); i++) {
		blocked = bool:Protection_GetPlayerParamInt(playerid, Protection:i, PROTECTION_PLAYER_BLOCK_UPDATE);
		if (blocked) {
			return 0;
		}
	}

	#if defined Protection_OnTrailerUpdate
		return Protection_OnTrailerUpdate(playerid, vehicleid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnTrailerUpdate
	#undef OnTrailerUpdate
#else
	#define _ALS_OnTrailerUpdate
#endif

#define OnTrailerUpdate Protection_OnTrailerUpdate
#if defined Protection_OnTrailerUpdate
	forward Protection_OnTrailerUpdate(playerid, vehicleid);
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

		// paused check
		if (Protection_IsPlayerPaused(playerid)) {
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

forward _Protection_SysreqFix();
public _Protection_SysreqFix()
{
	new temp[2];
	format(temp, sizeof(temp), "");
}

stock Protection_GivePlayerWarning(playerid, Protection:protection, const fstring[], {Float, _}:...)
{
	static
		warn_max,
		warn_count;

	warn_max = Protection_GetParamInt(protection, PROTECTION_MAX_WARNINGS);

	warn_count = Protection_GetPlayerParamInt(playerid, protection, PROTECTION_PLAYER_WARNING_COUNT);
	warn_count++;

	Protection_SetPlayerParamInt(playerid, protection, PROTECTION_PLAYER_WARNING_COUNT, warn_count);

	// format
	static const
		BYTES_PER_CELL = 4,
		STATIC_ARGS = 3;

	static
		args,
		arg_start,
		arg_end,
		message[144];

	#emit LOAD.S.pri 8
	#emit STOR.pri args

	if (args > STATIC_ARGS * BYTES_PER_CELL) {
		#emit ADDR.pri fstring
		#emit STOR.pri arg_start

		for (arg_end = arg_start + (args - STATIC_ARGS * BYTES_PER_CELL); arg_end > arg_start; arg_end -= BYTES_PER_CELL) {
			#emit LREF.pri arg_end
			#emit PUSH.pri
		}
		#emit PUSH.S fstring
		#emit PUSH.C 144
		#emit PUSH.C message
		#emit PUSH.C args
		#emit SYSREQ.C format

		#emit LCTRL 5
		#emit SCTRL 4

		CallRemoteFunction("OnPlayerGetProtectionWarning", "iiis", playerid, _:protection, warn_count >= warn_max, message);
	} else {
		Protection_strcpy(message, fstring);

		if (message[0] == '\0') {
			message[0] = '\1';
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
	Array params
*/

stock Protection_SetParamArray(Protection:protection, ProtectionParams:param, const values[], const size = sizeof(values))
{
	if (protection == PROTECTION_ALL) {
		for (new i = 0; i < sizeof(gParam); i++) {
			memcpy(gParam[Protection:i][param], values, 0, size * 4, PROTECTION_MAX_AVAILABLE_IDS);
		}
	} else {
		memcpy(gParam[protection][param], values, 0, size * 4, PROTECTION_MAX_AVAILABLE_IDS);
	}
}

stock Protection_GetParamArray(Protection:protection, ProtectionParams:param, values[], size = sizeof(values))
{
	memcpy(values, gParam[protection][param], 0, PROTECTION_MAX_AVAILABLE_IDS, size * 4);
}

/*
	String params
*/

stock Protection_SetParamString(Protection:protection, ProtectionParams:param, const value[])
{
	if (protection == PROTECTION_ALL) {
		for (new i = 0; i < sizeof(gParam); i++) {
			Protection_strcpy(gParam[Protection:i][param], value, PROTECTION_MAX_STRING);
		}
	} else {
		Protection_strcpy(gParam[protection][param], value, PROTECTION_MAX_STRING);
	}
}

stock Protection_GetParamString(Protection:protection, ProtectionParams:param, value[], size = sizeof(value))
{
	Protection_strcpy(value, gParam[protection][param], size);
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

stock Protection_SetPlayerParamString(playerid, Protection:protection, ProtectionPlayerParams:param, const value[])
{
	if (protection == PROTECTION_ALL) {
		for (new i = 0; i < sizeof(gPlayerParam[]); i++) {
			Protection_strcpy(gPlayerParam[playerid][Protection:i][param], value, PROTECTION_MAX_STRING);
		}
	} else {
		Protection_strcpy(gPlayerParam[playerid][protection][param], value, PROTECTION_MAX_STRING);
	}
}

stock Protection_GetPlayerParamString(playerid, Protection:protection, ProtectionPlayerParams:param, value[], size = sizeof(value))
{
	Protection_strcpy(value, gPlayerParam[playerid][protection][param], size);
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

stock Protection_SetPlayerInfoString(playerid, ProtectionPlayerInfo:param, const value[])
{
	Protection_strcpy(gPlayerInfo[playerid][param], value, PROTECTION_MAX_STRING);
}

stock Protection_GetPlayerInfoString(playerid, ProtectionPlayerInfo:param, value[], size = sizeof(value))
{
	Protection_strcpy(value, gPlayerInfo[playerid][param], size);
}

/*
	Tick different
*/

stock Protection_GetTickDiff(newtick, oldtick)
{
	return newtick - oldtick;
}

/*
	strcpy implementation
*/

stock Protection_strcpy(result[], const source[], const size = sizeof(result))
{
	result[0] = 0;
	return strcat(result, source, size);
}
