/*

	About: armour protection
	Author: ziggi

*/

#if !defined _protection_included
	#error "Please include protection before armour"
#endif

#if defined _protection_armour_included
	#endinput
#endif

#define _protection_armour_included

/*
	Vars
*/

static
	Float:gPlayerArmour[MAX_PLAYERS];

/*
	Call function
*/

forward Protection_Armour(playerid);
public Protection_Armour(playerid)
{
	static
		Float:current_armour;

	GetPlayerArmour(playerid, current_armour);

	if (current_armour < gPlayerArmour[playerid]) {
		gPlayerArmour[playerid] = current_armour;
		return 0;
	}

	if (current_armour > gPlayerArmour[playerid]) {
		// set right armour
		if (Protection_GetParamInt(PROTECTION_ARMOUR, PROTECTION_COUNTERMEASURE)) {
			SetPlayerArmour(playerid, gPlayerArmour[playerid]);
		}

		// report
		Protection_GivePlayerWarning(playerid, PROTECTION_ARMOUR, "%.2f > %.2f", current_armour, gPlayerArmour[playerid]);
		return 1;
	}

	return 0;
}

/*
	OnPlayerSpawn
*/

public OnPlayerSpawn(playerid)
{
	gPlayerArmour[playerid] = 0.0;

	#if defined Protection_Armour_OnPlayerSpawn
		return Protection_Armour_OnPlayerSpawn(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
	#define _ALS_OnPlayerSpawn
#endif

#define OnPlayerSpawn Protection_Armour_OnPlayerSpawn
#if defined Protection_Armour_OnPlayerSpawn
	forward Protection_Armour_OnPlayerSpawn(playerid);
#endif

/*
	SetPlayerArmour
*/

stock PArmour_SetPlayerArmour(playerid, Float:armour)
{
	gPlayerArmour[playerid] = armour;
	return SetPlayerArmour(playerid, armour);
}
#if defined _ALS_SetPlayerArmour
	#undef SetPlayerArmour
#else
	#define _ALS_SetPlayerArmour
#endif

#define SetPlayerArmour PArmour_SetPlayerArmour

/*
	GetPlayerArmour
*/

stock PArmour_GetPlayerArmour(playerid, &Float:armour)
{
	armour = gPlayerArmour[playerid];
	return 1;
}
#if defined _ALS_GetPlayerArmour
	#undef GetPlayerArmour
#else
	#define _ALS_GetPlayerArmour
#endif

#define GetPlayerArmour PArmour_GetPlayerArmour
