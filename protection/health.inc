/*

	About: health protection
	Author: ziggi

*/

#if !defined _protection_included
	#error "Please include protection before health"
#endif

#if defined _protection_health_included
	#endinput
#endif

#define _protection_health_included
#pragma library protection_health

/*
	Vars
*/

static
	Float:gPlayerHealth[MAX_PLAYERS];

/*
	Call function
*/

forward Protection_Health(playerid);
public Protection_Health(playerid)
{
	new
		Float:current_health;

	GetPlayerHealth(playerid, current_health);

	if (current_health < gPlayerHealth[playerid]) {
		gPlayerHealth[playerid] = current_health;
		return 0;
	}

	if (current_health > gPlayerHealth[playerid]) {
		// set right health
		SetPlayerHealth(playerid, gPlayerHealth[playerid]);

		// report
		Protection_GivePlayerWarning(playerid, PROTECTION_HEALTH, "%.2f > %.2f", current_health, gPlayerHealth[playerid]);
		return 1;
	}

	return 0;
}

/*
	OnPlayerSpawn
*/

public OnPlayerSpawn(playerid)
{
	gPlayerHealth[playerid] = 100.0;

	#if defined Protection_Health_OnPlayerSpawn
		return Protection_Health_OnPlayerSpawn(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
	#define _ALS_OnPlayerSpawn
#endif
 
#define OnPlayerSpawn Protection_Health_OnPlayerSpawn
#if defined Protection_Health_OnPlayerSpawn
	forward Protection_Health_OnPlayerSpawn(playerid);
#endif

/*
	SetPlayerHealth
*/

stock PHealth_SetPlayerHealth(playerid, Float:health)
{
	gPlayerHealth[playerid] = health;
	return SetPlayerHealth(playerid, health);
}
#if defined _ALS_SetPlayerHealth
	#undef SetPlayerHealth
#else
	#define _ALS_SetPlayerHealth
#endif
 
#define SetPlayerHealth PHealth_SetPlayerHealth

/*
	GetPlayerHealth
*/

stock PHealth_GetPlayerHealth(playerid, &Float:health)
{
	health = gPlayerHealth[playerid];
	return 1;
}
#if defined _ALS_GetPlayerHealth
	#undef GetPlayerHealth
#else
	#define _ALS_GetPlayerHealth
#endif
 
#define GetPlayerHealth PHealth_GetPlayerHealth
