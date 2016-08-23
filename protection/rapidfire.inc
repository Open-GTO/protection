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
		250, // 1
		250, // 2
		250, // 3
		250, // 4
		250, // 5
		250, // 6
		250, // 7
		250, // 8
		30, // 9
		250, // 10
		250, // 11
		250, // 12
		250, // 13
		250, // 14
		250, // 15
		500, // 16
		500, // 17
		500, // 18
		0, // 19
		0, // 20
		0, // 21
		160, // 22
		120, // 23
		1200, // 24
		800, // 25
		120, // 26
		120, // 27
		50, // 28
		90, // 29
		90, // 30
		90, // 31
		70, // 32
		1500, // 33
		1500, // 34
		1500, // 35
		1500, // 36
		500, // 37
		20, // 38
		500, // 39
		500, // 40
		10, // 41
		10, // 42
		500, // 43
		500, // 44
		500, // 45
		500 // 46
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
			shot_time,
			max_shot_time;

		current_tick = GetTickCount();
		shot_time = Protection_GetTickDiff(current_tick, gPlayerLastTick[playerid]);
		gPlayerLastTick[playerid] = current_tick;
		max_shot_time = Protection_GetWeaponShotTime(weaponid);

		if (shot_time < max_shot_time) {
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
