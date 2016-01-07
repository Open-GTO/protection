/*

	About: teleport protection
	Author: ziggi

*/

#if !defined _protection_included
	#error "Please include protection before teleport"
#endif

#if !defined _getspawninfo_included
	#error "Please include getspawninfo before teleport"
#endif

#if defined _protection_teleport_included
	#endinput
#endif

#define _protection_teleport_included
#pragma library protection_teleport

/*
	Vars
*/

static
	Float:gPrevPos[MAX_PLAYERS][3],
	Float:gOldPos[MAX_PLAYERS][3];

/*
	Call function
*/

forward Protection_Teleport(playerid);
public Protection_Teleport(playerid)
{
	// calculate distance
	static
		Float:pos_x,
		Float:pos_y,
		Float:pos_z,
		Float:distance_prev,
		Float:distance_old,
		Float:max_distance;

	GetPlayerPos(playerid, pos_x, pos_y, pos_z);
	
	distance_prev = VectorSize(gPrevPos[playerid][0] - pos_x, gPrevPos[playerid][1] - pos_y, 0.0);
	distance_old = VectorSize(gOldPos[playerid][0] - pos_x, gOldPos[playerid][1] - pos_y, 0.0);

	// checks
	if (IsPlayerInAnyVehicle(playerid)) {
		max_distance = Protection_GetParamFloat(PROTECTION_TELEPORT, PROTECTION_MAX_VEHICLE_DISTANCE);
	} else {
		max_distance = Protection_GetParamFloat(PROTECTION_TELEPORT, PROTECTION_MAX_DISTANCE);
	}

	if (distance_prev > max_distance && distance_old > max_distance) {
		// set right pos
		SetPlayerPos(playerid, gOldPos[playerid][0], gOldPos[playerid][1], gOldPos[playerid][2]);

		// report
		Protection_GivePlayerWarning(playerid, PROTECTION_TELEPORT, "%.2f and %.2f > %.2f", distance_prev, distance_old, max_distance);
		return 1;
	}

	// update
	gOldPos[playerid][0] = pos_x;
	gOldPos[playerid][1] = pos_y;
	gOldPos[playerid][2] = pos_z;

	return 0;
}

/*
	OnPlayerSpawn
*/

public OnPlayerSpawn(playerid)
{
	new bool:is_enabled = Protection_GetParamInt(PROTECTION_TELEPORT, PROTECTION_ENABLED) != 0;

	if (is_enabled) {
		static
			Float:si_x,
			Float:si_y,
			Float:si_z;

		GetSpawnInfo(playerid, _, _, si_x, si_y, si_z, _, _, _, _, _, _, _);
		
		gPrevPos[playerid][0] = si_x;
		gPrevPos[playerid][1] = si_y;
		gPrevPos[playerid][2] = si_z;

		gOldPos[playerid][0] = si_x;
		gOldPos[playerid][1] = si_y;
		gOldPos[playerid][2] = si_z;
	}

	#if defined PTeleport_OnPlayerSpawn
		return PTeleport_OnPlayerSpawn(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
	#define _ALS_OnPlayerSpawn
#endif
 
#define OnPlayerSpawn PTeleport_OnPlayerSpawn
#if defined PTeleport_OnPlayerSpawn
	forward PTeleport_OnPlayerSpawn(playerid);
#endif

/*
	SetPlayerPos
*/

stock PTeleport_SetPlayerPos(playerid, Float:x, Float:y, Float:z)
{
	new bool:is_enabled = Protection_GetParamInt(PROTECTION_TELEPORT, PROTECTION_ENABLED) != 0;

	if (is_enabled) {
		GetPlayerPos(playerid, gPrevPos[playerid][0], gPrevPos[playerid][1], gPrevPos[playerid][2]);

		gOldPos[playerid][0] = x;
		gOldPos[playerid][1] = y;
		gOldPos[playerid][2] = z;
	}

	return SetPlayerPos(playerid, x, y, z);
}
#if defined _ALS_SetPlayerPos
	#undef SetPlayerPos
#else
	#define _ALS_SetPlayerPos
#endif
 
#define SetPlayerPos PTeleport_SetPlayerPos


/*
	SetVehiclePos
*/

stock PTeleport_SetVehiclePos(vehicleid, Float:x, Float:y, Float:z)
{
	new bool:is_enabled = Protection_GetParamInt(PROTECTION_TELEPORT, PROTECTION_ENABLED) != 0;

	if (is_enabled) {
		static
			Float:prev_x,
			Float:prev_y,
			Float:prev_z;

		GetVehiclePos(vehicleid, prev_x, prev_y, prev_z);

		foreach (new playerid : Player) {
			if (GetPlayerVehicleID(playerid) == vehicleid) {
				gPrevPos[playerid][0] = prev_x;
				gPrevPos[playerid][1] = prev_y;
				gPrevPos[playerid][2] = prev_z;

				gOldPos[playerid][0] = x;
				gOldPos[playerid][1] = y;
				gOldPos[playerid][2] = z;
			}
		}
	}

	return SetVehiclePos(vehicleid, x, y, z);
}
#if defined _ALS_SetVehiclePos
	#undef SetVehiclePos
#else
	#define _ALS_SetVehiclePos
#endif
 
#define SetVehiclePos PTeleport_SetVehiclePos
