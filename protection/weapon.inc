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
	gWeaponSlots[] = {
		0, // 0
		0, // 1
		1, // 2
		1, // 3
		1, // 4
		1, // 5
		1, // 6
		1, // 7
		1, // 8
		1, // 9
		10, // 10
		10, // 11
		10, // 12
		10, // 13
		10, // 14
		10, // 15
		8, // 16
		8, // 17
		8, // 18
		-1, // 19
		-1, // 20
		-1, // 21
		2, // 22
		2, // 23
		2, // 24
		3, // 25
		3, // 26
		3, // 27
		4, // 28
		4, // 29
		5, // 30
		5, // 31
		4, // 32
		6, // 33
		6, // 34
		7, // 35
		7, // 36
		7, // 37
		7, // 38
		8, // 39
		12, // 40
		9, // 41
		9, // 42
		9, // 43
		11, // 44
		11, // 45
		11 // 46
	};

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

		if (gPlayerWeapons[playerid][slot][e_Bullets] == 0 && ammo == 0) {
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
		GivePlayerWeapon(playerid, gPlayerWeapons[playerid][slot][e_ID], 0);

		// report
		Protection_GivePlayerWarning(playerid, PROTECTION_WEAPON, "weaponid %d != %d", gPlayerWeapons[playerid][slot][e_ID], weaponid);
	}

	// bullets
	if (gPlayerWeapons[playerid][slot][e_Bullets] < bullets) {
		// set right bullets
		SetPlayerAmmo(playerid, gPlayerWeapons[playerid][slot][e_ID], gPlayerWeapons[playerid][slot][e_Bullets]);

		// report
		Protection_GivePlayerWarning(playerid, PROTECTION_WEAPON, "bullets %d < %d", gPlayerWeapons[playerid][slot][e_Bullets], bullets);
	} else {
		gPlayerWeapons[playerid][slot][e_Bullets] = bullets;
	}

	return 0;
}

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
