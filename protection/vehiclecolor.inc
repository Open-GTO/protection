/*

	About: vehicle color protection
	Author: ziggi

*/

#if !defined _protection_included
	#error "Please include protection before vehiclecolor"
#endif

#if defined _protection_vehicleclr_included
	#endinput
#endif

#define _protection_vehicleclr_included

/*
	OnVehicleRespray
*/

public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
	new bool:is_enabled = Protection_GetParamInt(PROTECTION_VEHICLECOLOR, PROTECTION_ENABLED) != 0;

	if (is_enabled) {
		Protection_GivePlayerWarning(playerid, PROTECTION_VEHICLECOLOR, "respray");

		if (Protection_GetParamInt(PROTECTION_VEHICLECOLOR, PROTECTION_COUNTERACTION) && Protection_GetPlayerParamInt(playerid, PROTECTION_VEHICLECOLOR, PROTECTION_PLAYER_COUNTERACTION)) {
			return 0;
		}
	}

	#if defined PVeColor_OnVehicleRespray
		return PVeColor_OnVehicleRespray(playerid, vehicleid, color1, color2);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnVehicleRespray
	#undef OnVehicleRespray
#else
	#define _ALS_OnVehicleRespray
#endif

#define OnVehicleRespray PVeColor_OnVehicleRespray
#if defined PVeColor_OnVehicleRespray
	forward PVeColor_OnVehicleRespray(playerid, vehicleid, color1, color2);
#endif

/*
	OnVehiclePaintjob
*/

public OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
	new bool:is_enabled = Protection_GetParamInt(PROTECTION_VEHICLECOLOR, PROTECTION_ENABLED) != 0;

	if (is_enabled) {
		Protection_GivePlayerWarning(playerid, PROTECTION_VEHICLECOLOR, "paintjob");

		if (Protection_GetParamInt(PROTECTION_VEHICLECOLOR, PROTECTION_COUNTERACTION)) {
			return 0;
		}
	}

	#if defined PVeColor_OnVehiclePaintjob
		return PVeColor_OnVehiclePaintjob(playerid, vehicleid, paintjobid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnVehiclePaintjob
	#undef OnVehiclePaintjob
#else
	#define _ALS_OnVehiclePaintjob
#endif

#define OnVehiclePaintjob PVeColor_OnVehiclePaintjob
#if defined PVeColor_OnVehiclePaintjob
	forward PVeColor_OnVehiclePaintjob(playerid, vehicleid, paintjobid);
#endif
