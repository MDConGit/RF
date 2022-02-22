class RF_FunctionEntry
{
	private bool m_PasswordSet;
	private Class m_Class;
	private string m_Name;
	private string m_Password;
	
	void RF_FunctionEntry(string name, string password, Class className)
	{
		m_Name = name;
		m_Class = className;
		m_Password = password;
		
		if (m_Password != string.Empty)
			m_PasswordSet = true;
		else
			m_PasswordSet = false;
	}

	Class GetClass()
	{
		if (m_Class)
			return m_Class;
		
		GetRPCManager().GetLog().Log(string.Format("No class assigned to function %1", m_Name));
		return NULL;
	}

	string GetName()
	{
		return m_Name;
	}

	string GetPassword()
	{
		return m_Password;
	}

	bool IsProtected()
	{
		return m_Password != "";
	}

	bool SetPassword(string password)
	{
		if (m_PasswordSet)
		{
			return false;
		}
		
		m_Password = password;
		m_PasswordSet = true;
		return true;
	}
}