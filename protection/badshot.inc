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
		new bool:countermeasure = Protection_GetParamInt(PROTECTION_BADSHOT, PROTECTION_COUNTERACTION) && Protection_GetPlayerParamInt(playerid, PROTECTION_BADSHOT, PROTECTION_PLAYER_COUNTERACTION);

		if(!Protection_IsBulletWeapon(weaponid)) {
			Protection_GivePlayerWarning(playerid, PROTECTION_BADSHOT, "weaponid: %d, hittype: %d, hitid: %d", weaponid, hittype, hitid);
			if (countermeasure) {
				return 0;
			}
		}

		switch (hittype) {
			case BULLET_HIT_TYPE_NONE, BULLET_HIT_TYPE_PLAYER,
			     BULLET_HIT_TYPE_OBJECT, BULLET_HIT_TYPE_PLAYER_OBJECT: {
			}
			case BULLET_HIT_TYPE_VEHICLE: {
				new vehicleid = GetPlayerVehicleID(playerid);

				if (vehicleid == hitid) {
					Protection_GivePlayerWarning(playerid, PROTECTION_BADSHOT, "hittype: %d, hitid: %d, vehicleid: %d", hittype, hitid, vehicleid);
					if (countermeasure) {
						return 0;
					}
				}
			}
			default: {
				Protection_GivePlayerWarning(playerid, PROTECTION_BADSHOT, "hittype: %d, hitid: %d", hittype, hitid);
				if (countermeasure) {
					return 0;
				}
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

/*
	Protection_IsBulletWeapon
*/

stock Protection_IsBulletWeapon(weaponid)
{
	return (WEAPON_COLT45 <= weaponid <= WEAPON_SNIPER) || weaponid == WEAPON_MINIGUN;
}
