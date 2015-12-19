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
	Float:gPrevPos[MAX_PLAYERS][2],
	Float:gOldPos[MAX_PLAYERS][2];

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

	gOldPos[playerid][0] = pos_x;
	gOldPos[playerid][1] = pos_y;

	// checks
	if (IsPlayerInAnyVehicle(playerid)) {
		max_distance = Protection_GetParamFloat(PROTECTION_TELEPORT, PROTECTION_MAX_VEHICLE_DISTANCE);
	} else {
		max_distance = Protection_GetParamFloat(PROTECTION_TELEPORT, PROTECTION_MAX_DISTANCE);
	}

	if (distance_prev > max_distance && distance_old > max_distance) {
		Protection_GivePlayerWarning(playerid, PROTECTION_TELEPORT, "%.2f and %.2f > %.2f", distance_prev, distance_old, max_distance);
		return 1;
	}

	return 0;
}

/*
	OnPlayerSpawn
*/

public OnPlayerSpawn(playerid)
{
	static
		Float:si_x,
		Float:si_y;

	GetSpawnInfo(playerid, _, _, si_x, si_y, _, _, _, _, _, _, _, _);
	
	gPrevPos[playerid][0] = si_x;
	gPrevPos[playerid][1] = si_y;

	gOldPos[playerid][0] = si_x;
	gOldPos[playerid][1] = si_y;

	#if defined Protection_OnPlayerSpawn
		return Protection_OnPlayerSpawn(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
	#define _ALS_OnPlayerSpawn
#endif
 
#define OnPlayerSpawn Protection_OnPlayerSpawn
#if defined Protection_OnPlayerSpawn
	forward Protection_OnPlayerSpawn(playerid);
#endif

/*
	SetPlayerPos
*/

stock pt_tp_SetPlayerPos(playerid, Float:x, Float:y, Float:z)
{
	static Float:buf_z;
	GetPlayerPos(playerid, gPrevPos[playerid][0], gPrevPos[playerid][1], buf_z);

	gOldPos[playerid][0] = x;
	gOldPos[playerid][1] = y;

	return SetPlayerPos(playerid, x, y, z);
}
#if defined _ALS_SetPlayerPos
	#undef SetPlayerPos
#else
	#define _ALS_SetPlayerPos
#endif
 
#define SetPlayerPos pt_tp_SetPlayerPos


/*
	SetVehiclePos
*/

stock pt_tp_SetVehiclePos(vehicleid, Float:x, Float:y, Float:z)
{
	static
		Float:prev_x,
		Float:prev_y,
		Float:prev_z;

	GetVehiclePos(vehicleid, prev_x, prev_y, prev_z);

	foreach (new playerid : Player) {
		if (GetPlayerVehicleID(playerid) == vehicleid) {
			gPrevPos[playerid][0] = prev_x;
			gPrevPos[playerid][1] = prev_y;

			gOldPos[playerid][0] = x;
			gOldPos[playerid][1] = y;
		}
	}

	return SetVehiclePos(vehicleid, x, y, z);
}
#if defined _ALS_SetVehiclePos
	#undef SetVehiclePos
#else
	#define _ALS_SetVehiclePos
#endif
 
#define SetVehiclePos pt_tp_SetVehiclePos
