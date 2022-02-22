static bool IsMissionClient()
{
	Print("Call to IsMissionClient() - legacy CF code, please remove.");
	Print("This is advisory only, NOT an error.");

	string dump;
	DumpStackString(dump);
	Print("Source: " + dump);
	
	if (!GetGame() || GetGame().IsDedicatedServer())
		return false;

	return true;
}

static bool IsMissionHost()
{
	Print("Call to IsMissionHost() - legacy CF code, please remove.");
	Print("This is advisory only, NOT an error.");

	string dump;
	DumpStackString(dump);
	Print("Source: " + dump);

	if (!GetGame())
		return false;

	if (GetGame().IsServer() || !GetGame().IsMultiplayer())
		return true;
	
	return false;
}

static bool IsMissionOffline()
{
	Print("Call to IsMissionOffline() - legacy CF code, please remove.");
	Print("This is advisory only, NOT an error.");

	string dump;
	DumpStackString(dump);
	Print("Source: " + dump);

	if (!GetGame())
		return false;

	if (GetGame().IsServer() && !GetGame().IsMultiplayer())
		return true;
	
	return false;
}