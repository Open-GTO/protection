/*

	About: rcon protection
	Author: ziggi

*/

#if !defined _protection_included
	#error "Please include protection before rcon"
#endif

#if defined _protection_rcon_included
	#endinput
#endif

#define _protection_rcon_included

/*
	OnRconLoginAttempt
*/

public OnRconLoginAttempt(ip[], password[], success)
{
	new bool:is_enabled = Protection_GetParamInt(PROTECTION_RCON, PROTECTION_ENABLED) != 0;

	if (is_enabled && !success && Protection_GetParamInt(PROTECTION_RCON, PROTECTION_ENABLED)) {
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

	#if defined PRcon_OnRconLoginAttempt
		return PRcon_OnRconLoginAttempt(ip, password, success);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnRconLoginAttempt
	#undef OnRconLoginAttempt
#else
	#define _ALS_OnRconLoginAttempt
#endif

#define OnRconLoginAttempt PRcon_OnRconLoginAttempt
#if defined PRcon_OnRconLoginAttempt
	forward PRcon_OnRconLoginAttempt(ip[], password[], success);
#endif
