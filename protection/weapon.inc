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
#pragma library protection_weapon

/*
	Defines
*/

#define PROTECTION_MAX_WEAPON_SLOTS 13

/*
	Enums
*/

enum e_Weapon_Info {
	e_wID,
	e_wBullets,
}

/*
	Vars
*/

static
	gPlayerWeapons[MAX_PLAYERS][PROTECTION_MAX_WEAPON_SLOTS][e_Weapon_Info],
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

stock Protection_GivePlayerWeapon(playerid, weaponid, ammo)
{
	static
		slot;
	
	slot = Protection_GetWeaponSlot(weaponid);
	if (slot == -1) {
		return 0;
	}

	gPlayerWeapons[playerid][slot][e_wID] = weaponid;
	gPlayerWeapons[playerid][slot][e_wBullets] += ammo;

	return GivePlayerWeapon(playerid, weaponid, ammo);
}
#if defined _ALS_GivePlayerWeapon
	#undef GivePlayerWeapon
#else
	#define _ALS_GivePlayerWeapon
#endif
 
#define GivePlayerWeapon Protection_GivePlayerWeapon

/*
	ResetPlayerWeapons
*/

stock Protection_ResetPlayerWeapons(playerid)
{
	for (new slot = 0; i < PROTECTION_MAX_WEAPON_SLOTS; i++) {
		gPlayerWeapons[playerid][slot][e_wID] = 0;
		gPlayerWeapons[playerid][slot][e_wBullets] = 0;
	}

	return ResetPlayerWeapons(playerid);
}
#if defined _ALS_ResetPlayerWeapons
	#undef ResetPlayerWeapons
#else
	#define _ALS_ResetPlayerWeapons
#endif
 
#define ResetPlayerWeapons Protection_ResetPlayerWeapons

/*
	SetPlayerAmmo
*/

stock Protection_SetPlayerAmmo(playerid, weaponid, ammo)
{
	static
		slot;
	
	slot = Protection_GetWeaponSlot(weaponid);
	if (slot == -1) {
		return 0;
	}

	gPlayerWeapons[playerid][slot][e_wBullets] = ammo;

	return SetPlayerAmmo(playerid, weaponid, ammo);
}
#if defined _ALS_SetPlayerAmmo
	#undef SetPlayerAmmo
#else
	#define _ALS_SetPlayerAmmo
#endif
 
#define SetPlayerAmmo Protection_SetPlayerAmmo

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
	if (gPlayerWeapons[playerid][slot][e_wID] != weaponid) {
		// set right ammo
		GivePlayerWeapon(playerid, gPlayerWeapons[playerid][slot][e_wID], 0);

		// report
		Protection_GivePlayerWarning(playerid, PROTECTION_WEAPON, "weaponid %d != %d", gPlayerWeapons[playerid][slot][e_wID], weaponid);
	}
	
	// bullets
	if (gPlayerWeapons[playerid][slot][e_wBullets] < bullets) {
		// set right bullets
		SetPlayerAmmo(playerid, gPlayerWeapons[playerid][slot][e_wID], gPlayerWeapons[playerid][slot][e_wBullets]);

		// report
		Protection_GivePlayerWarning(playerid, PROTECTION_WEAPON, "bullets %d < %d", gPlayerWeapons[playerid][slot][e_wBullets], bullets);
	}

	return 0;
}

/*
	OnPlayerExitVehicle
*/

public OnPlayerExitVehicle(playerid, vehicleid)
{
	static
		Float:pos_x,
		Float:pos_y,
		Float:pos_z;

	GetPlayerPos(playerid, pos_x, pos_y, pos_z);

	if (pos_z >= 55.0) {
		switch ( GetVehicleModel(vehicleid) ) {
			case 592,577,511,512,520,593,553,476,519,460,513,548,425,417,487,488,497,563,447,469: {
				GivePlayerWeapon(playerid, WEAPON_PARACHUTE, 1);
			}
		}
	}

	#if defined Protection_OnPlayerExitVehlicle
		return Protection_OnPlayerExitVehlicle(playerid, vehicleid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerExitVehlicle
	#undef OnPlayerExitVehlicle
#else
	#define _ALS_OnPlayerExitVehlicle
#endif
 
#define OnPlayerExitVehlicle Protection_OnPlayerExitVehlicle
#if defined Protection_OnPlayerExitVehlicle
	forward Protection_OnPlayerExitVehlicle(playerid, vehicleid);
#endif

/*
	Protection_GetWeaponSlot
*/

static stock Protection_GetWeaponSlot(weaponid)
{
	if (weaponid < 0 || weaponid > sizeof(gWeaponSlots)) {
		return -1;
	}

	return gWeaponSlots[weaponid];
}
