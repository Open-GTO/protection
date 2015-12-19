# protection
Flexible server protection system (development)

# Callbacks
```Pawn
forward OnPlayerGetProtectionWarning(playerid, Protection:protection, bool:warn_reached, message[]);
```

# Functions
```Pawn
// protection params
Protection_SetParamInt(Protection:protection, ProtectionParams:param, value)
Protection_GetParamInt(Protection:protection, ProtectionParams:param)
Protection_SetParamFloat(Protection:protection, ProtectionParams:param, Float:value)
Float:Protection_GetParamFloat(Protection:protection, ProtectionParams:param)
Protection_SetParamString(Protection:protection, ProtectionParams:param, value[])
Protection_GetParamString(Protection:protection, ProtectionParams:param, value[], size = sizeof(value))

// player protection params
Protection_SetPlayerParamInt(playerid, Protection:protection, ProtectionPlayerParams:param, value)
Protection_GetPlayerParamInt(playerid, Protection:protection, ProtectionPlayerParams:param)
Protection_SetPlayerParamFloat(playerid, Protection:protection, ProtectionPlayerParams:param, Float:value)
Protection_GetPlayerParamFloat(playerid, Protection:protection, ProtectionPlayerParams:param)
Protection_SetPlayerParamString(playerid, Protection:protection, ProtectionParams:param, value[])
Protection_GetPlayerParamString(playerid, Protection:protection, ProtectionParams:param, value[], size = sizeof(value))

// player info
Protection_SetPlayerInfoInt(playerid, ProtectionPlayerInfo:param, value)
Protection_GetPlayerInfoInt(playerid, ProtectionPlayerInfo:param)
Protection_SetPlayerInfoFloat(playerid, ProtectionPlayerParams:param, Float:value)
Float:Protection_GetPlayerInfoFloat(playerid, ProtectionPlayerParams:param)
Protection_SetPlayerInfoString(playerid, ProtectionParams:param, value[])
Protection_GetPlayerInfoString(playerid, ProtectionParams:param, value[], size = sizeof(value))
```

# Function params
```Pawn
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
```

# Examples
```Pawn
#include <a_samp>
#include "foreach"
#include "protection"

main() {}

public OnGameModeInit()
{
	Protection_SetParamInt(PROTECTION_ALL, PROTECTION_ENABLED, 1);
	Protection_SetParamString(PROTECTION_TELEPORT, PROTECTION_NAME, "Телепорт"); // redefine name
	return 1;
}

public OnPlayerGetProtectionWarning(playerid, Protection:protection, bool:warn_reached, message[])
{
	new
		string[144],
		protection_name[32];

	Protection_GetParamString(PROTECTION_TELEPORT, PROTECTION_NAME, protection_name);

	format(string, sizeof(string), "* Player: %d | Warn reached: %d | Name: %s | Message: %s", playerid, _:warn_reached, protection_name, message);
	SendClientMessageToAll(-1, string);
	return 1;
}
```
