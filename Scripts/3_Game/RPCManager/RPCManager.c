class RPCManager
{
	protected int m_LogEnabled = 0;
	protected ref map<string, ref RF_ModEntry> m_RegisteredMods = new map<string, ref RF_ModEntry>;
	protected ref RF_RPCLog m_Log;
	
	void RPCManager()
	{
		string cli;
		if (GetCLIParam("dorpclogs", cli))
			m_LogEnabled = cli.ToInt();
	}

	bool DoLogs()
	{
		return m_LogEnabled > 0;
	}

	bool DoLogsVerbose()
	{
		return m_LogEnabled > 1;
	}

	protected bool FindMod(string name, out RF_ModEntry result)
	{
		result = m_RegisteredMods[name];
		if (result)
			return true;

		return false;
	}

	RF_RPCLog GetLog()
	{
		if (!m_Log)
			m_Log = new RF_RPCLog();
		
		return m_Log;
	}

	void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		if (rpc_type != RF_RPC_ID)
		{
			GetLog().Log("RPC ID is not handled by RF, illegal operation.");
			return;
		}
		
		RFHeader headerRF;
		if (!ctx.Read(headerRF))
		{
			GetLog().Log("RF header not found - RPC ID conflict?");
			return;
		}
		
		string modName = headerRF.param1;
		string functionName = headerRF.param2;
		string password = headerRF.param3;

		GetLog().LogOnRPC(sender, modName, functionName, target);

		if (modName == "" || functionName == "")
		{
			GetLog().Log("Invalid RPC data  - RPC ID conflict?");
			return;
		}

		RF_ModEntry modEntry;
		if (!m_RegisteredMods.Find(modName, modEntry))
		{
			GetLog().LogMissing(modName);
			return;
		}
		
		RF_FunctionEntry functionEntry;		
		if (!modEntry.FindFunction(functionName, functionEntry))
		{
			GetLog().LogMissing(modName, functionName);
			return;
		}
		
		if (functionEntry.IsProtected() && password != functionEntry.GetPassword())
		{
			GetLog().LogInvalidPassword(modName, functionName);
			return;
		}
		
		if (functionEntry.GetClass())
		{
			int callType = CallType.Server;
			if (GetGame().IsClient())
				callType = CallType.Client;
			
			RPCCallFunctionParams callData = RPCCallFunctionParams( callType, ctx, sender, target );
			GetGame().GameScript.CallFunctionParams( functionEntry.GetClass(), functionName, NULL, callData );
		}
	}

	protected bool RegisterMod(RF_ModEntry mod)
	{
		if (!m_RegisteredMods.Contains(mod.GetName()))
		{
			if (m_RegisteredMods.Insert(mod.GetName(), mod))
				return true;
			
			GetRPCManager().GetLog().LogRegisterRPCFailure(mod.GetName());
		}
		
		GetRPCManager().GetLog().LogRegisterRPCExists(mod.GetName());
		return false;
	}

	bool RegisterRPC(string modName, string functionName, string password, Class targetClass)
	{
		RF_ModEntry modEntry;
		RF_FunctionEntry functionEntry;
		
		if (!FindMod(modName, modEntry))
		{
			modEntry = new RF_ModEntry(modName);
			RegisterMod(modEntry);
		}
		
		if (!modEntry.FindFunction(functionName, functionEntry))
		{
			functionEntry = new RF_FunctionEntry(functionName, password, targetClass);
			if (modEntry.RegisterFunction(functionEntry))
				return true;
		}

		return false;
	}

	void SendSecureRPC(string modName, string functionName, string password, Param params = NULL, bool guaranteed = false, PlayerIdentity recipient = NULL, Object target = NULL)
	{
		GetLog().LogSendRPC(modName, functionName, recipient, target, password != string.Empty);

		array<ref Param> rpcData = new array<ref Param>;
		RFHeader rfHeader = new RFHeader(modName, functionName, password);
		
		rpcData.Insert( rfHeader );
		rpcData.Insert( params );

		GetGame().RPC(target, RF_RPC_ID, rpcData, guaranteed, recipient);
	}

	bool SetPassword(string modName, string password)
	{
		RF_ModEntry modEntry;
		
		if (!FindMod(modName, modEntry))
			return false;

		if (!modEntry.SetPassword(password))
		{
			GetRPCManager().GetLog().LogPasswordAlreadySet(modName);
			return false;
		}
		
		return true;
	}

	bool SetPassword(string modName, string functionName, string password)
	{
		RF_ModEntry modEntry;
		RF_FunctionEntry functionEntry;

		if (!FindMod(modName, modEntry))
			return false;

		if (!modEntry.FindFunction(functionName, functionEntry))
			return false;

		if (!functionEntry.SetPassword(password))
		{
			GetRPCManager().GetLog().LogPasswordAlreadySet(modName, functionName);
			return false;
		}
		
		return true;
	}

	// Compatibility
	bool AddRPC(string modName, string functionName, Class targetClass, int executionScope = SingleplayerExecutionType.Server, string password = "")
	{
		return RegisterRPC(modName, functionName, password, targetClass);
	}

	void SendRPC(string modName, string functionName, Param params = NULL, bool guaranteed = false, PlayerIdentity recipient = NULL, Object target = NULL)
	{
		SendSecureRPC(modName, functionName, "", params, guaranteed, recipient, target);
	}
}