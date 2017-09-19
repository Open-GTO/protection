/*

	About: camera mode protection
	Author: ziggi

*/

#if !defined _protection_included
	#error "Please include protection before camera"
#endif

#if defined _protection_cameramode_included
	#endinput
#endif

#define _protection_cameramode_included

/*
	Call function
*/

forward Protection_CameraMode(playerid);
public Protection_CameraMode(playerid)
{
	new camera_mode = GetPlayerCameraMode(playerid);

	// checks
	if (!Protection_IsValidCameraMode(GetPlayerCameraMode(playerid))) {
		// report
		Protection_GivePlayerWarning(playerid, PROTECTION_CAMERA, "camera mode: %d", camera_mode);
		return 1;
	}

	return 0;
}

/*
	Protection_IsValidCameraMode
*/

stock Protection_IsValidCameraMode(camera_mode)
{
	new available_modes[PROTECTION_MAX_AVAILABLE_IDS];
	Protection_GetParamArray(PROTECTION_CAMERAMODE, PROTECTION_AVAILABLE_IDS, available_modes);

	for (new i = 0; i < sizeof(available_modes); i++) {
		if (available_modes[i] == -1) {
			break;
		}

		if (camera_mode == available_modes[i]) {
			return 1;
		}
	}

	return false;
}
