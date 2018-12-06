/*

	About: weapon protection
	Author: ziggi

*/

#if !defined _protection_included
	#error "Please include protection before weapon"
#endif

#if defined _protection_weapon_included
	#endinput
#endif

#define _protection_weapon_included

/*
	Defines
*/

#define PROTECTION_MAX_WEAPON_SLOTS 13
#define PROTECTION_PARACHUTE_SLOT_ID 11

// additional checks count if player got zero bullets
#if !defined PROTECTION_WEAPON_ZERO_BULLETS
	#define PROTECTION_WEAPON_ZERO_BULLETS 3
#endif

#if PROTECTION_WEAPON_ZERO_BULLETS < 0 || PROTECTION_WEAPON_ZERO_BULLETS > 255
	#error PROTECTION_WEAPON_ZERO_BULLETS should be in 0..255 range
#endif

/*
	Enums
*/

enum e_PT_WEAPON_INFO {
	e_ID,
	e_Bullets,
}

/*
	Vars
*/

static
	gPlayerWeapons[MAX_PLAYERS][PROTECTION_MAX_WEAPON_SLOTS][e_PT_WEAPON_INFO],
	gPlayerZeroBulletsChecks[MAX_PLAYERS char],
	gWeaponSlots[] = {
		0, // 0
		0, // 1 - WEAPON_BRASSKNUCKLE
		1, // 2 - WEAPON_GOLFCLUB
		1, // 3 - WEAPON_NITESTICK
		1, // 4 - WEAPON_KNIFE
		1, // 5 - WEAPON_BAT
		1, // 6 - WEAPON_SHOVEL
		1, // 7 - WEAPON_POOLSTICK
		1, // 8 - WEAPON_KATANA
		1, // 9 - WEAPON_CHAINSAW
		10, // 10 - WEAPON_DILDO
		10, // 11 - WEAPON_DILDO2
		10, // 12 - WEAPON_VIBRATOR
		10, // 13 - WEAPON_VIBRATOR2
		10, // 14 - WEAPON_FLOWER
		10, // 15 - WEAPON_CANE
		8, // 16 - WEAPON_GRENADE
		8, // 17 - WEAPON_TEARGAS
		8, // 18 - WEAPON_MOLTOV
		-1, // 19
		-1, // 20
		-1, // 21
		2, // 22 - WEAPON_COLT45
		2, // 23 - WEAPON_SILENCED
		2, // 24 - WEAPON_DEAGLE
		3, // 25 - WEAPON_SHOTGUN
		3, // 26 - WEAPON_SAWEDOFF
		3, // 27 - WEAPON_SHOTGSPA
		4, // 28 - WEAPON_UZI
		4, // 29 - WEAPON_MP5
		5, // 30 - WEAPON_AK47
		5, // 31 - WEAPON_M4
		4, // 32 - WEAPON_TEC9
		6, // 33 - WEAPON_RIFLE
		6, // 34 - WEAPON_SNIPER
		7, // 35 - WEAPON_ROCKETLAUNCHER
		7, // 36 - WEAPON_HEATSEEKER
		7, // 37 - WEAPON_FLAMETHROWER
		7, // 38 - WEAPON_MINIGUN
		8, // 39 - WEAPON_SATCHEL
		12, // 40 - WEAPON_BOMB
		9, // 41 - WEAPON_SPRAYCAN
		9, // 42 - WEAPON_FIREEXTINGUISHER
		9, // 43 - WEAPON_CAMERA
		11, // 44
		11, // 45
		11 // 46
	};

/*
	Call function
*/

forward Protection_Weapon(playerid);
public Protection_Weapon(playerid)
{
	static
		weaponid,
		bullets,
		slot;

	weaponid = GetPlayerWeapon(playerid);
	bullets = GetPlayerAmmo(playerid);
	slot = Protection_GetWeaponSlot(weaponid);

	if (weaponid == 0) {
		return 0;
	}

	// weapon
	if (gPlayerWeapons[playerid][slot][e_ID] != weaponid) {
		// set right weapon
		if (Protection_GetParamInt(PROTECTION_WEAPON, PROTECTION_COUNTERMEASURE)) {
			GivePlayerWeapon(playerid, gPlayerWeapons[playerid][slot][e_ID], 0);
		}

		// report
		Protection_GivePlayerWarning(playerid, PROTECTION_WEAPON, "weaponid %d != %d", gPlayerWeapons[playerid][slot][e_ID], weaponid);
	}

	// bullets
	if (gPlayerWeapons[playerid][slot][e_Bullets] < bullets) {
		// set right bullets
		if (Protection_GetParamInt(PROTECTION_WEAPON, PROTECTION_COUNTERMEASURE)) {
			ResetPlayerWeapons(playerid);
			for (new i = 0; i < sizeof(gPlayerWeapons[]); i++) {
				GivePlayerWeapon(playerid, gPlayerWeapons[playerid][i][e_ID], gPlayerWeapons[playerid][i][e_Bullets]);
			}
		}

		// report
		Protection_GivePlayerWarning(playerid, PROTECTION_WEAPON, "bullets %d < %d", gPlayerWeapons[playerid][slot][e_Bullets], bullets);
	} else {
		if (bullets == 0) {
			gPlayerZeroBulletsChecks{playerid}++;

			if (gPlayerZeroBulletsChecks{playerid} >= PROTECTION_WEAPON_ZERO_BULLETS) {
				gPlayerZeroBulletsChecks{playerid} = 0;
				gPlayerWeapons[playerid][slot][e_Bullets] = bullets;
			}
		} else {
			gPlayerWeapons[playerid][slot][e_Bullets] = bullets;
			gPlayerZeroBulletsChecks{playerid} = 0;
		}
	}

	return 0;
}

/*
	GivePlayerWeapon
*/

stock PWeapon_GivePlayerWeapon(playerid, weaponid, ammo)
{
	new bool:is_enabled = Protection_GetParamInt(PROTECTION_WEAPON, PROTECTION_ENABLED) != 0;

	if (is_enabled) {
		static
			slot;

		slot = Protection_GetWeaponSlot(weaponid);
		if (slot == -1) {
			return 0;
		}

		switch (slot) {
			case 3, 4, 5: {
				gPlayerWeapons[playerid][slot][e_Bullets] += ammo;
			}
			default: {
				gPlayerWeapons[playerid][slot][e_Bullets] = ammo;
			}
		}

		gPlayerWeapons[playerid][slot][e_ID] = weaponid;
	}

	return GivePlayerWeapon(playerid, weaponid, ammo);
}
#if defined _ALS_GivePlayerWeapon
	#undef GivePlayerWeapon
#else
	#define _ALS_GivePlayerWeapon
#endif

#define GivePlayerWeapon PWeapon_GivePlayerWeapon

/*
	ResetPlayerWeapons
*/

stock PWeapon_ResetPlayerWeapons(playerid)
{
	new bool:is_enabled = Protection_GetParamInt(PROTECTION_WEAPON, PROTECTION_ENABLED) != 0;

	if (is_enabled) {
		for (new slot = 0; slot < PROTECTION_MAX_WEAPON_SLOTS; slot++) {
			gPlayerWeapons[playerid][slot][e_ID] = 0;
			gPlayerWeapons[playerid][slot][e_Bullets] = 0;
		}
	}

	return ResetPlayerWeapons(playerid);
}
#if defined _ALS_ResetPlayerWeapons
	#undef ResetPlayerWeapons
#else
	#define _ALS_ResetPlayerWeapons
#endif

#define ResetPlayerWeapons PWeapon_ResetPlayerWeapons

/*
	SetPlayerAmmo
*/

stock PWeapon_SetPlayerAmmo(playerid, weaponid, ammo)
{
	new bool:is_enabled = Protection_GetParamInt(PROTECTION_WEAPON, PROTECTION_ENABLED) != 0;

	if (is_enabled) {
		static
			slot;

		slot = Protection_GetWeaponSlot(weaponid);
		if (slot == -1) {
			return 0;
		}

		gPlayerWeapons[playerid][slot][e_Bullets] = ammo;
	}

	return SetPlayerAmmo(playerid, weaponid, ammo);
}
#if defined _ALS_SetPlayerAmmo
	#undef SetPlayerAmmo
#else
	#define _ALS_SetPlayerAmmo
#endif

#define SetPlayerAmmo PWeapon_SetPlayerAmmo

/*
	OnPlayerSpawn
*/

public OnPlayerSpawn(playerid)
{
	new bool:is_enabled = Protection_GetParamInt(PROTECTION_WEAPON, PROTECTION_ENABLED) != 0;

	if (is_enabled) {
		static
			slot,
			weapons[3][e_PT_WEAPON_INFO];

		GetSpawnInfo(playerid, _, _, _, _, _, _,
		             weapons[0][e_ID], weapons[0][e_Bullets],
		             weapons[1][e_ID], weapons[1][e_Bullets],
		             weapons[2][e_ID], weapons[2][e_Bullets]);

		for (new i = 0; i < sizeof(weapons); i++) {
			slot = Protection_GetWeaponSlot(weapons[i][e_ID]);
			if (slot == -1) {
				continue;
			}

			gPlayerWeapons[playerid][slot][e_ID] = weapons[i][e_ID];
			gPlayerWeapons[playerid][slot][e_Bullets] = weapons[i][e_Bullets];
		}

		gPlayerZeroBulletsChecks{playerid} = 0;
	}

	#if defined PWeapon_OnPlayerSpawn
		return PWeapon_OnPlayerSpawn(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
	#define _ALS_OnPlayerSpawn
#endif

#define OnPlayerSpawn PWeapon_OnPlayerSpawn
#if defined PWeapon_OnPlayerSpawn
	forward PWeapon_OnPlayerSpawn(playerid);
#endif

/*
	OnPlayerExitVehicle
*/

public OnPlayerExitVehicle(playerid, vehicleid)
{
	new bool:is_enabled = Protection_GetParamInt(PROTECTION_WEAPON, PROTECTION_ENABLED) != 0;

	if (is_enabled) {
		switch (GetVehicleModel(vehicleid)) {
			case 592, 577, 511, 512, 520, 593, 553, 476, 519, 460,
			     513, 548, 425, 417, 487, 488, 497, 563, 447, 469: {
				gPlayerWeapons[playerid][PROTECTION_PARACHUTE_SLOT_ID][e_ID] = WEAPON_PARACHUTE;
				gPlayerWeapons[playerid][PROTECTION_PARACHUTE_SLOT_ID][e_Bullets] = 1;
			}
		}
	}
	
	#if defined PWeapon_OnPlayerExitVehicle
		return PWeapon_OnPlayerExitVehicle(playerid, vehicleid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerExitVehicle
	#undef OnPlayerExitVehicle
#else
	#define _ALS_OnPlayerExitVehicle
#endif

#define OnPlayerExitVehicle PWeapon_OnPlayerExitVehicle
#if defined PWeapon_OnPlayerExitVehicle
	forward PWeapon_OnPlayerExitVehicle(playerid, vehicleid);
#endif

/*
	Protection_GetWeaponSlot
*/

stock Protection_GetWeaponSlot(weaponid)
{
	if (weaponid < 0 || weaponid > sizeof(gWeaponSlots)) {
		return -1;
	}

	return gWeaponSlots[weaponid];
}
