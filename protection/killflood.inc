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

public OnPlayerDeath(playerid, killerid, reason)
{
	new bool:is_enabled = Protection_GetParamInt(PROTECTION_KILLFLOOD, PROTECTION_ENABLED) != 0;

	if (is_enabled && playerid == killerid) {
		Protection_GivePlayerWarning(playerid, PROTECTION_KILLFLOOD, "reason: %d", reason);
		return 0;
	}

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
