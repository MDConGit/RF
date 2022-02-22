RPCManager GetRPCManager()
{
	return g_Game.GetRPCManager();
}

modded class DayZGame extends CGame
{
	ref RPCManager m_RPCManager;

	RPCManager GetRPCManager()
	{
		if (m_RPCManager)
		{
			return m_RPCManager;
		}
		else
		{
			m_RPCManager = new RPCManager();
			if (m_RPCManager)
				return m_RPCManager;
			else
				return NULL;
		}
	}

	void DayZGame()
	{
	}

	override void DeferredInit()
	{
		super.DeferredInit();
		
		m_RPCManager = new RPCManager();
	}

	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		if (rpc_type == RF_RPC_ID)
		{
			GetRPCManager().OnRPC(sender, target, rpc_type, ctx);
			return;
		}
		
		super.OnRPC(sender, target, rpc_type, ctx);
	}
}