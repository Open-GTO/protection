/*

	About: ping protection
	Author: ziggi

*/

#if !defined _protection_included
	#error "Please include protection before ping"
#endif

#if defined _protection_ping_included
	#endinput
#endif

#define _protection_ping_included

/*
	Call function
*/

forward Protection_Ping(playerid);
public Protection_Ping(playerid)
{
	static
		ping,
		max_ping;

	ping = Protection_GetPlayerInfoInt(playerid, PROTECTION_PLAYER_PING);
	max_ping = Protection_GetParamInt(PROTECTION_PING, PROTECTION_MAX_PING);

	if (ping > max_ping) {
		Protection_GivePlayerWarning(playerid, PROTECTION_PING, "%d > %d", ping, max_ping);
		return 1;
	}

	return 0;
}
