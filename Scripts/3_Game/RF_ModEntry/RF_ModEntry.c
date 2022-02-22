class RF_ModEntry
{
	private bool m_PasswordSet;
	private string m_Name;
	private ref map<string, ref RF_FunctionEntry> m_Functions;

	void RF_ModEntry(string name)
	{
		m_Name = name;
		m_Functions = new map<string, ref RF_FunctionEntry>;
		m_PasswordSet = false;
	}
	
	bool FindFunction(string name, out RF_FunctionEntry result)
	{
		result = m_Functions[name];
		if (result)
			return true;

		return false;
	}

	map<string, ref RF_FunctionEntry> GetFunctions()
	{
		if (m_Functions.Count() > 0)
			return m_Functions;
		
		return NULL;
	}

	int GetFunctionsCount()
	{
		return m_Functions.Count();
	}

	string GetName()
	{
		return m_Name;
	}

	bool IsPasswordSet()
	{
		return m_PasswordSet;
	}

	bool RegisterFunction(RF_FunctionEntry function)
	{
		if (!m_Functions.Contains(function.GetName()))
		{
			if (m_Functions.Insert(function.GetName(), function))
			{
				GetRPCManager().GetLog().LogRegisterRPC(m_Name, function.GetName(), function.IsProtected());
				return true;
			}
			
			GetRPCManager().GetLog().LogRegisterRPCFailure(m_Name, function.GetName());
		}
		
		GetRPCManager().GetLog().LogRegisterRPCExists(m_Name, function.GetName());
		return false;
	}

	bool SetPassword(string password)
	{
		if (m_PasswordSet)
			return false;
		
		foreach (string name, ref RF_FunctionEntry functionEntry : m_Functions)
		{
			if(!functionEntry.SetPassword(password))
				GetRPCManager().GetLog().LogPasswordAlreadySet(m_Name, functionEntry.GetName());
		}

		m_PasswordSet = true;
		return true;
	}
}