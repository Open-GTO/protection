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
		Float:distance_old;

	max_distance = Protection_GetParamFloat(PROTECTION_CAMERA, PROTECTION_MAX_DISTANCE);
	GetPlayerCameraPos(playerid, pos_x, pos_y, pos_z);

	Protection_GetPlayerPrevPos(playerid, x, y, z);
	distance_prev = VectorSize(x - pos_x, y - pos_y, z - pos_z);

	Protection_GetPlayerOldPos(playerid, x, y, z);
	distance_old = VectorSize(x - pos_x, y - pos_y, z - pos_z);

	// checks
	if (distance_prev > max_distance && distance_old > max_distance) {
		// set right pos
		SetCameraBehindPlayer(playerid);

		// report
		Protection_GivePlayerWarning(playerid, PROTECTION_CAMERA, "%.2f and %.2f > %.2f", distance_prev, distance_old, max_distance);
		return 1;
	}

	return 0;
}
