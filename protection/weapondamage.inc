/*

	About: weapondamage protection
	Author: ziggi

*/

#if !defined _protection_included
	#error "Please include protection before weapondamage"
#endif

#if defined _protection_weapondmg_included
	#endinput
#endif

#define _protection_weapondmg_included

/*
	Vars
*/

static
	Float:gWeaponDamages[] = {
		1.0, // 0
		1.0, // 1 - WEAPON_BRASSKNUCKLE
		1.0, // 2 - WEAPON_GOLFCLUB
		1.0, // 3 - WEAPON_NITESTICK
		1.0, // 4 - WEAPON_KNIFE
		1.0, // 5 - WEAPON_BAT
		1.0, // 6 - WEAPON_SHOVEL
		1.0, // 7 - WEAPON_POOLSTICK
		1.0, // 8 - WEAPON_KATANA
		1.0, // 9 - WEAPON_CHAINSAW
		1.0, // 10 - WEAPON_DILDO
		1.0, // 11 - WEAPON_DILDO2
		1.0, // 12 - WEAPON_VIBRATOR
		1.0, // 13 - WEAPON_VIBRATOR2
		1.0, // 14 - WEAPON_FLOWER
		1.0, // 15 - WEAPON_CANE
		82.5, // 16 - WEAPON_GRENADE
		0.0, // 17 - WEAPON_TEARGAS
		1.0, // 18 - WEAPON_MOLTOV
		0.0, // 19
		0.0, // 20
		0.0, // 21
		8.25, // 22 - WEAPON_COLT45
		13.2, // 23 - WEAPON_SILENCED
		46.2, // 24 - WEAPON_DEAGLE
		3.3, // 25 - WEAPON_SHOTGUN
		3.3, // 26 - WEAPON_SAWEDOFF
		4.95, // 27 - WEAPON_SHOTGSPA
		6.6, // 28 - WEAPON_UZI
		8.25, // 29 - WEAPON_MP5
		9.9, // 30 - WEAPON_AK47
		9.9, // 31 - WEAPON_M4
		6.6, // 32 - WEAPON_TEC9
		24.75, // 33 - WEAPON_RIFLE
		41.25, // 34 - WEAPON_SNIPER
		82.5, // 35 - WEAPON_ROCKETLAUNCHER
		82.5, // 36 - WEAPON_HEATSEEKER
		1.0, // 37 - WEAPON_FLAMETHROWER
		46.2, // 38 - WEAPON_MINIGUN
		82.5, // 39 - WEAPON_SATCHEL
		0.0, // 40 - WEAPON_BOMB
		0.33, // 41 - WEAPON_SPRAYCAN
		0.33, // 42 - WEAPON_FIREEXTINGUISHER
		0.0, // 43 - WEAPON_CAMERA
		0.0, // 44
		0.0, // 45
		0.0 // 46
	};

/*
	OnPlayerGiveDamage
*/

public OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart)
{
	new bool:is_enabled = Protection_GetParamInt(PROTECTION_WEAPONDAMAGE, PROTECTION_ENABLED) != 0;

	if (is_enabled && damagedid != INVALID_PLAYER_ID) {
		static
			Float:health,
			Float:armour,
			Float:difference,
			Float:damage;

		Protection_GetWeaponDamage(weaponid, damage);

		if (amount != damage) {
			Protection_GivePlayerWarning(playerid, PROTECTION_WEAPONDAMAGE, "%.02f != %.02f", amount, damage);
		}

		GetPlayerHealth(damagedid, health);
		GetPlayerArmour(damagedid, armour);

		if (armour > 0.0) {
			difference = armour - damage;
		}

		if (difference >= 0.0) {
			SetPlayerArmour(damagedid, armour - damage);
		} else {
			SetPlayerArmour(damagedid, 0.0);
			SetPlayerHealth(damagedid, health + difference);
		}
	}

	#if defined PWeapondmg_OnPlayerGiveDamage
		return PWeapondmg_OnPlayerGiveDamage(playerid, damagedid, amount, weaponid, bodypart);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerGiveDamage
	#undef OnPlayerGiveDamage
#else
	#define _ALS_OnPlayerGiveDamage
#endif

#define OnPlayerGiveDamage PWeapondmg_OnPlayerGiveDamage
#if defined PWeapondmg_OnPlayerGiveDamage
	forward PWeapondmg_OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart);
#endif

/*
	Protection_GetWeaponDamage
*/

stock Protection_GetWeaponDamage(weaponid, &Float:damage)
{
	if (weaponid < 0 || weaponid > sizeof(gWeaponDamages)) {
		return 0;
	}

	damage = gWeaponDamages[weaponid];
	return 1;
}
