/*

	About: money protection
	Author: ziggi

*/

#if !defined _protection_included
	#error "Please include protection before money"
#endif

#if defined _protection_money_included
	#endinput
#endif

#define _protection_money_included

/*
	Vars
*/

static
	gPlayerMoney[MAX_PLAYERS];

/*
	Call function
*/

forward Protection_Money(playerid);
public Protection_Money(playerid)
{
	static
		current_money;

	current_money = GetPlayerMoney(playerid);

	if (current_money < gPlayerMoney[playerid]) {
		gPlayerMoney[playerid] = current_money;
		return 0;
	}

	if (current_money > gPlayerMoney[playerid]) {
		// set right money
		ResetPlayerMoney(playerid);
		GivePlayerMoney(playerid, gPlayerMoney[playerid]);

		// report
		Protection_GivePlayerWarning(playerid, PROTECTION_MONEY, "%d > %d", current_money, gPlayerMoney[playerid]);
		return 1;
	}

	return 0;
}

/*
	GivePlayerMoney
*/

stock PMoney_GivePlayerMoney(playerid, Float:money)
{
	gPlayerMoney[playerid] += money;
	return GivePlayerMoney(playerid, money);
}
#if defined _ALS_GivePlayerMoney
	#undef GivePlayerMoney
#else
	#define _ALS_GivePlayerMoney
#endif

#define GivePlayerMoney PMoney_GivePlayerMoney

/*
	ResetPlayerMoney
*/

stock PMoney_ResetPlayerMoney(playerid)
{
	gPlayerMoney[playerid] = 0;
}
#if defined _ALS_ResetPlayerMoney
	#undef ResetPlayerMoney
#else
	#define _ALS_ResetPlayerMoney
#endif

#define ResetPlayerMoney PMoney_ResetPlayerMoney

/*
	GetPlayerMoney
*/

stock PMoney_GetPlayerMoney(playerid)
{
	return gPlayerMoney[playerid];
}
#if defined _ALS_GetPlayerMoney
	#undef GetPlayerMoney
#else
	#define _ALS_GetPlayerMoney
#endif

#define GetPlayerMoney PMoney_GetPlayerMoney
