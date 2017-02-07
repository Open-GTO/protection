/*

	About: killflood protection
	Author: ziggi

*/

#if !defined _protection_included
	#error "Please include protection before killflood"
#endif

#if defined _protection_killflood_included
	#endinput
#endif

#define _protection_killflood_included

/*
	Vars
*/

static
	bool:gPlayerIsDead[MAX_PLAYERS char];

/*
	OnPlayerConnect
*/

public OnPlayerConnect(playerid)
{
	gPlayerIsDead{playerid} = true;

	#if defined PKillflood_OnPlayerConnect
		return PKillflood_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect PKillflood_OnPlayerConnect
#if defined PKillflood_OnPlayerConnect
	forward PKillflood_OnPlayerConnect(playerid);
#endif

/*
	OnPlayerSpawn
*/

public OnPlayerSpawn(playerid)
{
	gPlayerIsDead{playerid} = false;

	#if defined PKillflood_OnPlayerSpawn
		return PKillflood_OnPlayerSpawn(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
	#define _ALS_OnPlayerSpawn
#endif

#define OnPlayerSpawn PKillflood_OnPlayerSpawn
#if defined PKillflood_OnPlayerSpawn
	forward PKillflood_OnPlayerSpawn(playerid);
#endif

/*
	OnPlayerDeath
*/

public OnPlayerDeath(playerid, killerid, reason)
{
	new bool:is_enabled = Protection_GetParamInt(PROTECTION_KILLFLOOD, PROTECTION_ENABLED) != 0;

	if (is_enabled && gPlayerIsDead{playerid}) {
		Protection_GivePlayerWarning(playerid, PROTECTION_KILLFLOOD, "reason: %d", reason);
		return 0;
	}

	gPlayerIsDead{playerid} = true;

	#if defined PKillflood_OnPlayerDeath
		return PKillflood_OnPlayerDeath(playerid, killerid, reason);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerDeath
	#undef OnPlayerDeath
#else
	#define _ALS_OnPlayerDeath
#endif

#define OnPlayerDeath PKillflood_OnPlayerDeath
#if defined PKillflood_OnPlayerDeath
	forward PKillflood_OnPlayerDeath(playerid, killerid, reason);
#endif
