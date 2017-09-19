# Protection
Flexible server protection system (development).

# Dependencies
- [foreach](https://github.com/Open-GTO/foreach)
- [getspawninfo](https://github.com/Open-GTO/getspawninfo)

# Callbacks
```Pawn
forward OnPlayerGetProtectionWarning(playerid, Protection:protection, bool:warn_reached, message[]);
```

# Functions
```Pawn
// protection params
Protection_SetParamInt(Protection:protection, ProtectionParams:param, value);
Protection_GetParamInt(Protection:protection, ProtectionParams:param);
Protection_SetParamFloat(Protection:protection, ProtectionParams:param, Float:value);
Float:Protection_GetParamFloat(Protection:protection, ProtectionParams:param);
Protection_SetParamString(Protection:protection, ProtectionParams:param, value[]);
Protection_GetParamString(Protection:protection, ProtectionParams:param, value[], size = sizeof(value));
Protection_SetParamArray(Protection:protection, ProtectionParams:param, values[], const size = sizeof(values));
Protection_GetParamArray(Protection:protection, ProtectionParams:param, values[], size = sizeof(values));

// player protection params
Protection_SetPlayerParamInt(playerid, Protection:protection, ProtectionPlayerParams:param, value);
Protection_GetPlayerParamInt(playerid, Protection:protection, ProtectionPlayerParams:param);
Protection_SetPlayerParamFloat(playerid, Protection:protection, ProtectionPlayerParams:param, Float:value);
Float:Protection_GetPlayerParamFloat(playerid, Protection:protection, ProtectionPlayerParams:param);
Protection_SetPlayerParamString(playerid, Protection:protection, ProtectionPlayerParams:param, value[]);
Protection_GetPlayerParamString(playerid, Protection:protection, ProtectionPlayerParams:param, value[], size = sizeof(value));

// player info
Protection_SetPlayerInfoInt(playerid, ProtectionPlayerInfo:param, value);
Protection_GetPlayerInfoInt(playerid, ProtectionPlayerInfo:param);
Protection_SetPlayerInfoFloat(playerid, ProtectionPlayerInfo:param, Float:value);
Float:Protection_GetPlayerInfoFloat(playerid, ProtectionPlayerInfo:param);
Protection_SetPlayerInfoString(playerid, ProtectionPlayerInfo:param, value[]);
Protection_GetPlayerInfoString(playerid, ProtectionPlayerInfo:param, value[], size = sizeof(value));
```

# Function params
```Pawn
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
```

# Examples
```Pawn
#include <a_samp>
#include "foreach"
#include "getspawninfo"
#include "protection"

main() {}

public OnGameModeInit()
{
	// enable all protections
	Protection_SetParamInt(PROTECTION_ALL, PROTECTION_ENABLED, 1);
	// redefine name
	Protection_SetParamString(PROTECTION_TELEPORT, PROTECTION_NAME, "Телепорт");
	return 1;
}

public OnPlayerSpawn(playerid)
{
	// disable all protections for admin
	if (IsPlayerAdmin(playerid)) {
		Protection_SetPlayerParamInt(playerid, PROTECTION_ALL, PROTECTION_PLAYER_DISABLED, 1);
	}
	return 1;
}

public OnPlayerGetProtectionWarning(playerid, Protection:protection, bool:warn_reached, message[])
{
	new
		string[144],
		protection_name[32];

	Protection_GetParamString(protection, PROTECTION_NAME, protection_name);

	format(string, sizeof(string), "* Player: %d | Warn reached: %d | Name: %s | Message: %s", playerid, _:warn_reached, protection_name, message);
	SendClientMessageToAll(-1, string);
	return 1;
}
```
