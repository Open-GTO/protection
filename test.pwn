#include <a_samp>
#include <foreach>
#include <getspawninfo>
#include <weaponskill>
#include "protection"

main() {
	// enable all protections
	Protection_SetParamInt(PROTECTION_ALL, PROTECTION_ENABLED, 1);
	// redefine name
	Protection_SetParamString(PROTECTION_TELEPORT, PROTECTION_NAME, "Телепорт");
}

public OnPlayerSpawn(playerid)
{
	// disable all protections for admin
	if (IsPlayerAdmin(playerid)) {
		Protection_SetPlayerParamInt(playerid, PROTECTION_ALL, PROTECTION_PLAYER_DISABLED, 1);
	}
	return 1;
}

public OnPlayerGetProtectionWarning(playerid, Protection:protection, bool:warn_reached, message[])
{
	new
		string[144],
		protection_name[32];

	Protection_GetParamString(protection, PROTECTION_NAME, protection_name);

	format(string, sizeof(string), "* Player: %d | Warn reached: %d | Name: %s | Message: %s", playerid, _:warn_reached, protection_name, message);
	SendClientMessageToAll(-1, string);
	return 1;
}