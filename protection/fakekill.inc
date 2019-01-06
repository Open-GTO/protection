/*

	About: fakekill protection
	Author: Nexius
	Contributor: ziggi

*/

#if !defined _protection_included
	#error "Please include protection before fakekill"
#endif

#if defined _protection_fakekill_included
	#endinput
#endif

#define _protection_fakekill_included

/*
	Defines
*/

#define PROTECTION_WEAPON_HELICOPTER (50)

/*
	OnPlayerDeath
*/

public OnPlayerDeath(playerid, killerid, reason)
{
	new bool:is_enabled = Protection_GetParamInt(PROTECTION_FAKEKILL, PROTECTION_ENABLED) != 0;

	if (is_enabled && killerid != INVALID_PLAYER_ID) {
		static
			killer_state,
			killer_weapon;

		killer_state = GetPlayerState(killerid);
		killer_weapon = GetPlayerWeapon(killerid);

		if (!IsPlayerStreamedIn(playerid, killerid)) {
			Protection_GivePlayerWarning(playerid, PROTECTION_FAKEKILL, "reason: %d", reason);
			return 0;
		}

		if (killer_state != PLAYER_STATE_DRIVER) {
			if(reason == WEAPON_VEHICLE || reason == PROTECTION_WEAPON_HELICOPTER) {
				Protection_GivePlayerWarning(playerid, PROTECTION_FAKEKILL, "reason: %d, state: %d", reason, killer_state);
				return 0;
			}

			if (killer_weapon != reason && reason <= WEAPON_PARACHUTE && reason != WEAPON_FLAMETHROWER) {
				Protection_GivePlayerWarning(playerid, PROTECTION_FAKEKILL, "reason: %d, weapon: %d", reason, killer_weapon);
				return 0;
			}
		}
	}

	#if defined PFakekill_OnPlayerDeath
		return PFakekill_OnPlayerDeath(playerid, killerid, reason);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerDeath
	#undef OnPlayerDeath
#else
	#define _ALS_OnPlayerDeath
#endif

#define OnPlayerDeath PFakekill_OnPlayerDeath
#if defined PFakekill_OnPlayerDeath
	forward PFakekill_OnPlayerDeath(playerid, killerid, reason);
#endif
