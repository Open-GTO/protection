/*

	About: specialaction protection
	Author: ziggi

*/

#if !defined _protection_included
	#error "Please include protection before specialaction"
#endif

#if defined _protection_rcon_included
	#endinput
#endif

#define _protection_rcon_included
#pragma library protection_rcon

/*
	OnRconLoginAttempt
*/

public OnRconLoginAttempt(ip[], password[], success)
{
	if (!success && Protection_GetParamInt(PROTECTION_RCON, PROTECTION_ENABLED)) {
		new
			player_ip[PROTECTION_MAX_IP + 1];

		foreach (new playerid : Player) {
			Protection_GetPlayerInfoString(playerid, PROTECTION_PLAYER_IP, player_ip);

			if (!strcmp(ip, player_ip, false)) {
				Protection_GivePlayerWarning(playerid, PROTECTION_RCON, "IP: %s | Password: %s", player_ip, password);
				break;
			}
		}
	}

	#if defined Protection_OnRconLoginAttempt
		return Protection_OnRconLoginAttempt(ip, password, success);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnRconLoginAttempt
	#undef OnRconLoginAttempt
#else
	#define _ALS_OnRconLoginAttempt
#endif
 
#define OnRconLoginAttempt Protection_OnRconLoginAttempt
#if defined Protection_OnRconLoginAttempt
	forward Protection_OnRconLoginAttempt(ip[], password[], success);
#endif
