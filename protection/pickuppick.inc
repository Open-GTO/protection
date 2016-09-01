/*

	About: pickuppick protection
	Author: ziggi

*/

#if !defined _protection_included
	#error "Please include protection before pickuppick"
#endif

#if defined _protection_pickuppick_included
	#endinput
#endif

#define _protection_pickuppick_included

/*
	OnPlayerPickUpPickup
*/

public OnPlayerPickUpPickup(playerid, pickupid)
{
	new bool:is_enabled = Protection_GetParamInt(PROTECTION_PICKUPPICK, PROTECTION_ENABLED) != 0;

	if (is_enabled) {
		new
			Float:pos_x, Float:pos_y, Float:pos_z,
			Float:x, Float:y, Float:z,
			Float:distance_prev,
			Float:distance_old,
			Float:max_distance;

		max_distance = Protection_GetParamFloat(PROTECTION_PICKUPPICK, PROTECTION_MAX_DISTANCE);
		GetPlayerPos(playerid, pos_x, pos_y, pos_z);

		Protection_GetPlayerPrevPos(playerid, x, y, z);
		distance_prev = VectorSize(x - pos_x, y - pos_y, z - pos_z);

		Protection_GetPlayerOldPos(playerid, x, y, z);
		distance_old = VectorSize(x - pos_x, y - pos_y, z - pos_z);

		if (distance_old > max_distance && distance_prev > max_distance) {
			// report
			Protection_GivePlayerWarning(playerid, PROTECTION_PICKUPPICK, "%.2f and %.2f > %.2f", distance_old, distance_prev, max_distance);
			return 0;
		}
	}

	#if defined PPickupick_OnPlayerPickUpPickup
		return PPickupick_OnPlayerPickUpPickup(playerid, pickupid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerPickUpPickup
	#undef OnPlayerPickUpPickup
#else
	#define _ALS_OnPlayerPickUpPickup
#endif

#define OnPlayerPickUpPickup PPickupick_OnPlayerPickUpPickup
#if defined PPickupick_OnPlayerPickUpPickup
	forward PPickupick_OnPlayerPickUpPickup(playerid, pickupid);
#endif
