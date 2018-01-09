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
	Enums
*/

enum e_PT_PICKUP_INFO {
	bool:e_IsValid,
	e_Model,
	e_Type,
	Float:e_PosX,
	Float:e_PosY,
	Float:e_PosZ,
	e_World,
}

/*
	Vars
*/

static
	gPickups[MAX_PICKUPS][e_PT_PICKUP_INFO];

/*
	CreatePickup
*/

stock Protection_CreatePickup(model, type, Float:X, Float:Y, Float:Z, virtualworld = 0)
{
	new pickupid = CreatePickup(model, type, X, Y, Z, virtualworld);
	if (pickupid == -1) {
		return -1;
	}

	gPickups[pickupid][e_IsValid] = true;
	gPickups[pickupid][e_Model] = model;
	gPickups[pickupid][e_Type] = type;
	gPickups[pickupid][e_PosX] = X;
	gPickups[pickupid][e_PosY] = Y;
	gPickups[pickupid][e_PosZ] = Z;
	gPickups[pickupid][e_World] = virtualworld;
	return pickupid;
}
#if defined _ALS_CreatePickup
	#undef CreatePickup
#else
	#define _ALS_CreatePickup
#endif

#define CreatePickup Protection_CreatePickup

/*
	DestroyPickup
*/

stock Protection_DestroyPickup(pickupid)
{
	gPickups[pickupid][e_IsValid] = false;
	return DestroyPickup(pickupid);
}
#if defined _ALS_DestroyPickup
	#undef DestroyPickup
#else
	#define _ALS_DestroyPickup
#endif

#define DestroyPickup Protection_DestroyPickup

/*
	OnPlayerPickUpPickup
*/

public OnPlayerPickUpPickup(playerid, pickupid)
{
	new bool:is_enabled = Protection_GetParamInt(PROTECTION_PICKUPPICK, PROTECTION_ENABLED) != 0;

	if (is_enabled) {
		static
			Float:x, Float:y, Float:z,
			Float:distance_prev,
			Float:distance_old,
			Float:max_distance;

	#if defined Streamer_GetItemStreamerID
		new streamerid = Streamer_GetItemStreamerID(playerid, STREAMER_TYPE_PICKUP, pickupid);
	#endif
		max_distance = Protection_GetParamFloat(PROTECTION_PICKUPPICK, PROTECTION_MAX_DISTANCE);

		Protection_GetPlayerPrevPos(playerid, x, y, z);
		if (Protection_IsValidPickup(pickupid)) {
			Protection_GetPickupDistance(pickupid, x, y, z, distance_prev);
		}
	#if defined IsValidDynamicPickup && defined Streamer_GetDistanceToItem
		else if (IsValidDynamicPickup(streamerid)) {
			Streamer_GetDistanceToItem(x, y, z, STREAMER_TYPE_PICKUP, streamerid, distance_prev);
		}
	#endif

		Protection_GetPlayerOldPos(playerid, x, y, z);
		if (Protection_IsValidPickup(pickupid)) {
			Protection_GetPickupDistance(pickupid, x, y, z, distance_old);
		}
	#if defined IsValidDynamicPickup && defined Streamer_GetDistanceToItem
		else if (IsValidDynamicPickup(streamerid)) {
			Streamer_GetDistanceToItem(x, y, z, STREAMER_TYPE_PICKUP, streamerid, distance_old);
		}
	#endif

		if (distance_old > max_distance && distance_prev > max_distance) {
			// report
			Protection_GivePlayerWarning(playerid, PROTECTION_PICKUPPICK, "%.2f and %.2f > %.2f",
			                             distance_old, distance_prev, max_distance);
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

/*
	Pickup functions
*/

stock Protection_IsValidPickup(pickupid)
{
	return 0 <= pickupid < MAX_PICKUPS && gPickups[pickupid][e_IsValid];
}

stock Protection_GetPickupWorld(pickupid)
{
	if (!Protection_IsValidPickup(pickupid)) {
		return -1;
	}

	return gPickups[pickupid][e_World];
}

stock Protection_GetPickupType(pickupid)
{
	if (!Protection_IsValidPickup(pickupid)) {
		return -1;
	}

	return gPickups[pickupid][e_Type];
}

stock Protection_GetPickupModel(pickupid)
{
	if (!Protection_IsValidPickup(pickupid)) {
		return -1;
	}

	return gPickups[pickupid][e_Model];
}

stock Protection_GetPickupPos(pickupid, &Float:x, &Float:y, &Float:z)
{
	if (!Protection_IsValidPickup(pickupid)) {
		return 0;
	}

	x = gPickups[pickupid][e_PosX];
	y = gPickups[pickupid][e_PosY];
	z = gPickups[pickupid][e_PosZ];
	return 1;
}

stock Protection_GetPickupDistance(pickupid, Float:x, Float:y, Float:z, &Float:distance)
{
	if (!Protection_IsValidPickup(pickupid)) {
		return 0;
	}

	distance = VectorSize(x - gPickups[pickupid][e_PosX],
	                      y - gPickups[pickupid][e_PosY],
	                      z - gPickups[pickupid][e_PosZ]);
	return 1;
}
