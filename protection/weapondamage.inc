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
		1.0, // 1
		1.0, // 2
		1.0, // 3
		1.0, // 4
		1.0, // 5
		1.0, // 6
		1.0, // 7
		1.0, // 8
		1.0, // 9
		1.0, // 10
		1.0, // 11
		1.0, // 12
		1.0, // 13
		1.0, // 14
		1.0, // 15
		82.5, // 16
		0.0, // 17
		1.0, // 18
		0.0, // 19
		0.0, // 20
		0.0, // 21
		8.25, // 22
		13.2, // 23
		46.2, // 24
		3.3, // 25
		3.3, // 26
		4.95, // 27
		6.6, // 28
		8.25, // 29
		9.9, // 30
		9.9, // 31
		6.6, // 32
		24.75, // 33
		41.25, // 34
		82.5, // 35
		82.5, // 36
		1.0, // 37
		46.2, // 38
		82.5, // 39
		0.0, // 40
		0.33, // 41
		0.33, // 42
		0.0, // 43
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

static stock Protection_GetWeaponDamage(weaponid, &Float:damage)
{
	if (weaponid < 0 || weaponid > sizeof(gWeaponDamages)) {
		return 0;
	}

	damage = gWeaponDamages[weaponid];
	return 1;
}
