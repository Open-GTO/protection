/*

	About: bad shot protection
	Author: ziggi

*/

#if !defined _protection_included
	#error "Please include protection before badshot"
#endif

#if defined _protection_badshot_included
	#endinput
#endif

#define _protection_badshot_included

/*
	OnPlayerWeaponShot
*/

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	new bool:is_enabled = Protection_GetParamInt(PROTECTION_BADSHOT, PROTECTION_ENABLED) != 0;

	if (is_enabled) {
		switch (hittype) {
			case BULLET_HIT_TYPE_NONE, BULLET_HIT_TYPE_PLAYER,
			     BULLET_HIT_TYPE_OBJECT, BULLET_HIT_TYPE_PLAYER_OBJECT: {
			}
			case BULLET_HIT_TYPE_VEHICLE: {
				new vehicle = GetPlayerVehicleID(playerid);

				if(vehicle == hitid) {
					Protection_GivePlayerWarning(playerid, PROTECTION_BADSHOT, "hittype: %d, hitid: %d, vehicle: %d", hittype, hitid, vehicle);
					return 0;
				}
			}
			default: {
				Protection_GivePlayerWarning(playerid, PROTECTION_BADSHOT, "hittype: %d, hitid: %d", hittype, hitid);
				return 0;
			}
		}
	}

	#if defined PBadshot_OnPlayerWeaponShot
		return PBadshot_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerWeaponShot
	#undef OnPlayerWeaponShot
#else
	#define _ALS_OnPlayerWeaponShot
#endif

#define OnPlayerWeaponShot PBadshot_OnPlayerWeaponShot
#if defined PBadshot_OnPlayerWeaponShot
	forward PBadshot_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
#endif
