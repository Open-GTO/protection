/*

	About: vehicle tuning protection
	Author: Nexius
	Contributor: ziggi

*/

#if !defined _protection_included
	#error "Please include protection before vehicletuning"
#endif

#if defined _protection_vehicletun_included
	#endinput
#endif

#define _protection_vehicletun_included

/*
	OnVehicleMod
*/

public OnVehicleMod(playerid, vehicleid, componentid)
{
	new bool:is_enabled = Protection_GetParamInt(PROTECTION_VEHICLETUNING, PROTECTION_ENABLED) != 0;

	if (is_enabled) {
		if (GetPlayerInterior(playerid) == 0) {
			Protection_GivePlayerWarning(playerid, PROTECTION_VEHICLETUNING, "tuning");

			if (Protection_GetParamInt(PROTECTION_VEHICLETUNING, PROTECTION_COUNTERMEASURE) && Protection_GetPlayerParamInt(playerid, PROTECTION_VEHICLETUNING, PROTECTION_PLAYER_COUNTERMEASURE)) {
				return 0;
			}
		}
	}

	#if defined PVeTuning_OnVehicleMod
		return PVeTuning_OnVehicleMod(playerid, vehicleid, componentid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnVehicleMod
	#undef OnVehicleMod
#else
	#define _ALS_OnVehicleMod
#endif

#define OnVehicleMod PVeTuning_OnVehicleMod
#if defined PVeTuning_OnVehicleMod
	forward PVeTuning_OnVehicleMod(playerid, vehicleid, componentid);
#endif
