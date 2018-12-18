/*

	About: camera protection
	Author: ziggi

*/

#if !defined _protection_included
	#error "Please include protection before camera"
#endif

#if defined _protection_camera_included
	#endinput
#endif

#define _protection_camera_included

/*
	Vars
*/

static
	Float:gPrevPos[MAX_PLAYERS][3],
	Float:gOldPos[MAX_PLAYERS][3];

/*
	Call function
*/

forward Protection_Camera(playerid);
public Protection_Camera(playerid)
{
	// calculate distance
	static
		Float:max_distance,
		Float:pos_x, Float:pos_y, Float:pos_z,
		Float:x, Float:y, Float:z,
		Float:distance_prev,
		Float:distance_old,
		Float:distance_player_prev,
		Float:distance_player_old;

	max_distance = Protection_GetParamFloat(PROTECTION_CAMERA, PROTECTION_MAX_DISTANCE);
	
	GetPlayerCameraPos(playerid, pos_x, pos_y, pos_z);

	distance_prev = VectorSize(gPrevPos[playerid][0] - pos_x, gPrevPos[playerid][1] - pos_y, gPrevPos[playerid][2] - pos_z);
	distance_old = VectorSize(gOldPos[playerid][0] - pos_x, gOldPos[playerid][1] - pos_y, gOldPos[playerid][2] - pos_z);

	Protection_GetPlayerPrevPos(playerid, x, y, z);
	distance_player_prev = VectorSize(x - pos_x, y - pos_y, z - pos_z);

	Protection_GetPlayerOldPos(playerid, x, y, z);
	distance_player_old = VectorSize(x - pos_x, y - pos_y, z - pos_z);

	// checks
	if (
		distance_prev > max_distance
		&& distance_old > max_distance
		&& distance_player_prev > max_distance
		&& distance_player_old > max_distance
	) {
		// set right pos
		if (Protection_GetParamInt(PROTECTION_CAMERA, PROTECTION_COUNTERMEASURE) && Protection_GetPlayerParamInt(playerid, PROTECTION_CAMERA, PROTECTION_PLAYER_CM_DISABLED) != 1) {
			SetCameraBehindPlayer(playerid);
		}

		// report
		Protection_GivePlayerWarning(playerid, PROTECTION_CAMERA, "%.2f | %.2f | %.2f | %.2f > %.2f", distance_prev, distance_old, distance_player_prev, distance_player_old, max_distance);
		return 1;
	}

	// update
	gPrevPos[playerid][0] = pos_x;
	gPrevPos[playerid][1] = pos_y;
	gPrevPos[playerid][2] = pos_z;

	return 0;
}

/*
	SetPlayerCameraPos
*/

stock PCamera_SetPlayerCameraPos(playerid, Float:x, Float:y, Float:z)
{
	new bool:is_enabled = Protection_GetParamInt(PROTECTION_CAMERA, PROTECTION_ENABLED) != 0;

	if (is_enabled) {
		GetPlayerCameraPos(playerid, gPrevPos[playerid][0], gPrevPos[playerid][1], gPrevPos[playerid][2]);

		gOldPos[playerid][0] = x;
		gOldPos[playerid][1] = y;
		gOldPos[playerid][2] = z;
	}

	return SetPlayerCameraPos(playerid, x, y, z);
}
#if defined _ALS_SetPlayerCameraPos
	#undef SetPlayerCameraPos
#else
	#define _ALS_SetPlayerCameraPos
#endif

#define SetPlayerCameraPos PCamera_SetPlayerCameraPos

/*
	SetCameraBehindPlayer
*/

stock PCamera_SetCameraBehindPlayer(playerid)
{
	new bool:is_enabled = Protection_GetParamInt(PROTECTION_CAMERA, PROTECTION_ENABLED) != 0;

	if (is_enabled) {
		GetPlayerCameraPos(playerid, gPrevPos[playerid][0], gPrevPos[playerid][1], gPrevPos[playerid][2]);
		GetPlayerPos(playerid, gOldPos[playerid][0], gOldPos[playerid][1], gOldPos[playerid][2]);
	}

	return SetCameraBehindPlayer(playerid);
}
#if defined _ALS_SetCameraBehindPlayer
	#undef SetCameraBehindPlayer
#else
	#define _ALS_SetCameraBehindPlayer
#endif

#define SetCameraBehindPlayer PCamera_SetCameraBehindPlayer