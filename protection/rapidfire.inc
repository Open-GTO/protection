/*

	About: rapid fire protection
	Author: ziggi

*/

#if !defined _protection_included
	#error "Please include protection before rapidfire"
#endif

#if defined _protection_rapidfire_included
	#endinput
#endif

#define _protection_rapidfire_included

/*
	Vars
*/

static
	gPlayerLastTick[MAX_PLAYERS],
	gWeaponShotTimes[] = {
		250, // 0
		250, // 1 - WEAPON_BRASSKNUCKLE
		250, // 2 - WEAPON_GOLFCLUB
		250, // 3 - WEAPON_NITESTICK
		250, // 4 - WEAPON_KNIFE
		250, // 5 - WEAPON_BAT
		250, // 6 - WEAPON_SHOVEL
		250, // 7 - WEAPON_POOLSTICK
		250, // 8 - WEAPON_KATANA
		30, // 9 - WEAPON_CHAINSAW
		250, // 10 - WEAPON_DILDO
		250, // 11 - WEAPON_DILDO2
		250, // 12 - WEAPON_VIBRATOR
		250, // 13 - WEAPON_VIBRATOR2
		250, // 14 - WEAPON_FLOWER
		250, // 15 - WEAPON_CANE
		500, // 16 - WEAPON_GRENADE
		500, // 17 - WEAPON_TEARGAS
		500, // 18 - WEAPON_MOLTOV
		0, // 19
		0, // 20
		0, // 21
		160, // 22 - WEAPON_COLT45
		120, // 23 - WEAPON_SILENCED
		950, // 24 - WEAPON_DEAGLE
		800, // 25 - WEAPON_SHOTGUN
		120, // 26 - WEAPON_SAWEDOFF
		120, // 27 - WEAPON_SHOTGSPA
		70, // 28 - WEAPON_UZI
		70, // 29 - WEAPON_MP5
		80, // 30 - WEAPON_AK47
		80, // 31 - WEAPON_M4
		70, // 32 - WEAPON_TEC9
		1000, // 33 - WEAPON_RIFLE
		1000, // 34 - WEAPON_SNIPER
		1000, // 35 - WEAPON_ROCKETLAUNCHER
		1000, // 36 - WEAPON_HEATSEEKER
		500, // 37 - WEAPON_FLAMETHROWER
		0, // 38 - WEAPON_MINIGUN
		500, // 39 - WEAPON_SATCHEL
		500, // 40 - WEAPON_BOMB
		10, // 41 - WEAPON_SPRAYCAN
		10, // 42 - WEAPON_FIREEXTINGUISHER
		500, // 43 - WEAPON_CAMERA
		0, // 44
		0, // 45
		0 // 46
	};

/*
	OnPlayerWeaponShot
*/

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	new bool:is_enabled = Protection_GetParamInt(PROTECTION_RAPIDFIRE, PROTECTION_ENABLED) != 0;

	if (is_enabled) {
		static
			current_tick,
			ping,
			shot_time,
			max_shot_time;

		current_tick = GetTickCount();
		ping = Protection_GetPlayerInfoInt(playerid, PROTECTION_PLAYER_PING);
		shot_time = Protection_GetTickDiff(current_tick, gPlayerLastTick[playerid]);
		gPlayerLastTick[playerid] = current_tick;
		max_shot_time = Protection_GetWeaponShotTime(weaponid);

		if (max_shot_time != 0 && shot_time + ping < max_shot_time) {
			Protection_GivePlayerWarning(playerid, PROTECTION_RAPIDFIRE, "%d < %d", shot_time, max_shot_time);
			return 0;
		}
	}

	#if defined PRapidfire_OnPlayerWeaponShot
		return PRapidfire_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerWeaponShot
	#undef OnPlayerWeaponShot
#else
	#define _ALS_OnPlayerWeaponShot
#endif

#define OnPlayerWeaponShot PRapidfire_OnPlayerWeaponShot
#if defined PRapidfire_OnPlayerWeaponShot
	forward PRapidfire_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
#endif

/*
	Protection_GetWeaponShotTime
*/

stock Protection_GetWeaponShotTime(weaponid)
{
	if (weaponid < 0 || weaponid > sizeof(gWeaponShotTimes)) {
		return 0;
	}

	return gWeaponShotTimes[weaponid];
}
