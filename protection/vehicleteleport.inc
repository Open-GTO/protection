/*

	About: vehicle teleport protection
	Author: ziggi

*/

#if !defined _protection_included
	#error "Please include protection before vehicleteleport"
#endif

#if defined _protection_vehicletp_included
	#endinput
#endif

#define _protection_vehicletp_included
#pragma library protection_vehicletp

/*
	OnUnoccupiedVehicleU
*/

public OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat, Float:new_x, Float:new_y, Float:new_z, Float:vel_x, Float:vel_y, Float:vel_z)
{
	static
		Float:distance,
		Float:max_distance,
		Float:pos_x,
		Float:pos_y,
		Float:pos_z;

	GetVehiclePos(vehicleid, pos_x, pos_y, pos_z);
	distance = VectorSize(pos_x - new_x, pos_y - new_y, pos_z - new_z);
	max_distance = Protection_GetParamFloat(PROTECTION_VEHICLETELEPORT, PROTECTION_MAX_DISTANCE);

	if (distance > max_distance) {
		// set right pos
		SetVehiclePos(vehicleid, pos_x, pos_y, pos_z);

		// report
		Protection_GivePlayerWarning(playerid, PROTECTION_VEHICLETELEPORT, "%.2f > %.2f", distance, max_distance);
		return 0;
	}

	#if defined Protection_OnUnoccupiedVehicleU
		return Protection_OnUnoccupiedVehicleU(vehicleid, playerid, passenger_seat, new_x, new_y, new_z, vel_x, vel_y, vel_z);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnUnoccupiedVehicleUpdate
	#undef OnUnoccupiedVehicleUpdate
#else
	#define _ALS_OnUnoccupiedVehicleUpdate
#endif
 
#define OnUnoccupiedVehicleUpdate Protection_OnUnoccupiedVehicleU
#if defined Protection_OnUnoccupiedVehicleU
	forward Protection_OnUnoccupiedVehicleU(vehicleid, playerid, passenger_seat, Float:new_x, Float:new_y, Float:new_z, Float:vel_x, Float:vel_y, Float:vel_z);
#endif