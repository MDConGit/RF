class CfgPatches
{
	class RF_Scripts
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Scripts"
		};
	};
	class JM_CF_Scripts {};
	class RPC_Scripts {};
};
class CfgMods
{
	class RF_Mod
	{
		dir = "RF";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "RF";
		credits = "MDC";
		author = "MDC";
		authorID = "0";
		version = "1.0";
		extra = 0;
		type = "mod";
		dependencies[] = {"Game", "World", "Mission"};
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = {"RF/Scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"RF/Scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"RF/Scripts/5_Mission"};
			};
		};
	};
};