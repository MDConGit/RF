class RF_RPCLog extends Managed
{
	protected int 				m_Day, m_Hour, m_LastDay, m_Minute, m_Month, m_Second, m_Year = 0;
	protected FileHandle 		m_LogFileHandle;
	protected string			m_Date;
	protected string 			m_LogDirectory = "$profile:RF";
	
    void RF_RPCLog()
    {
		if (!GetGame().IsDedicatedServer())
			return;

		if (!FileExist(m_LogDirectory))
			MakeDirectory(m_LogDirectory);

		WriteLine("[RF] Session Begin");
		WriteLine("------------------");
	}
	
	protected string GetDate()
	{
		GetYearMonthDay(m_Year, m_Month, m_Day);
		
		if (m_LastDay != m_Day)
			m_Date = string.Format("%1-%2-%3", m_Year.ToStringLen(2), m_Month.ToStringLen(2), m_Day.ToStringLen(2));
		
		m_LastDay = m_Day;

		return m_Date;
	}
	
    protected string GetLogFile()
    {
        return string.Format("%1/%2.log", m_LogDirectory, GetDate());
    }

    protected string GetTime()
    {
        GetHourMinuteSecond(m_Hour, m_Minute, m_Second);
        
		return string.Format("%1:%2:%3", m_Hour.ToStringLen(2), m_Minute.ToStringLen(2), m_Second.ToStringLen(2));
    }

	void Log(string message)
	{
		WriteLine(message);
	}

	void LogInvalidPassword(string modName, string functionName)
	{
		WriteLine(string.Format("Invalid password provided for RPC (%1::%2)", modName, functionName));
	}

	void LogMissing(string modName, string functionName = "")
	{
		if (!GetRPCManager().DoLogsVerbose())
			return;

		if (functionName)
			WriteLine(string.Format("Mod (%1) not found", modName));
		else
			WriteLine(string.Format("Function (%1:%2) not found", modName, functionName));
	}
	
	void LogOnRPC(PlayerIdentity sender, string modName, string functionName, Object object)
	{
		if (!GetRPCManager().DoLogsVerbose())
			return;

		if (sender == NULL)
			WriteLine(string.Format("Received RPC from: Unknown/Server, ID: %1::%2 (%3)", modName, functionName, object));
		else
			WriteLine(string.Format("Received RPC from: %1 (%2), ID %3::%4 (%5)", sender.GetName(), sender.GetPlainId(), modName, functionName, object));
	}

	void LogRegisterRPC(string modName, string functionName, bool secure)
	{
		if (!GetRPCManager().DoLogsVerbose())
			return;
		
		if (secure)
			WriteLine(string.Format("Registered secure RPC (%1::%2)", modName, functionName));
		else
			WriteLine(string.Format("Registered RPC (%1::%2)", modName, functionName));
	}

	void LogRegisterRPCExists(string modName, string functionName = "")
	{
		if (!GetRPCManager().DoLogsVerbose())
			return;

		if (functionName)
			WriteLine(string.Format("Existing RPC (%1::%2) skipped", modName, functionName));
		else
			WriteLine(string.Format("Existing mod (%1) skipped", modName));
	}

	void LogRegisterRPCFailure(string modName, string functionName = "")
	{
		if (!GetRPCManager().DoLogsVerbose())
			return;

		if (functionName)
			WriteLine(string.Format("Could not register RPC (%1::%2)", modName, functionName));
		else
			WriteLine(string.Format("Could not register mod (%1)", modName));
	}

	void LogSendRPC(string modName, string functionName, PlayerIdentity recipient, Object object, bool secure)
	{
		string secureText = "";

		if (!GetRPCManager().DoLogsVerbose())
			return;

		if (secure)
			secureText = "secure ";

		if (recipient == NULL)
			WriteLine(string.Format("Sent %1RPC (%2::%3) to: Unknown/Server (%4)", secureText, modName, functionName, object));
		else
			WriteLine(string.Format("Sent %1RPC (%2::%3) to: %4 (%5) (%6)", secureText, modName, functionName, recipient.GetName(), recipient.GetPlainId(), object));
	}

	void LogPasswordAlreadySet(string modName, string functionName = "")
	{
		if (!GetRPCManager().DoLogsVerbose())
			return;

		if (functionName)
			WriteLine(string.Format("Password already set for %1:%2", modName, functionName));
		else
			WriteLine(string.Format("Password already set for %1", modName));
	}

    protected void WriteLine(string input)
    {
		if (!GetRPCManager().DoLogs())
			return;

		m_LogFileHandle = OpenFile(GetLogFile(), FileMode.APPEND);
        if (m_LogFileHandle != 0)
		{
			FPrintln(m_LogFileHandle, string.Format("[RF] [%1] %2", GetTime(), input));
			CloseFile(m_LogFileHandle);
		}
    }
}