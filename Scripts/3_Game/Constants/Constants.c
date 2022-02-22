#define RF

enum CallType
{
	Server,
	Client
}
enum SingleplayerExecutionType
{
	Server,
	Client,
	Both
}
enum SingeplayerExecutionType: SingleplayerExecutionType {}

const int RF_RPC_ID = 3500;

typedef Param3<string, string, string> RFHeader;
typedef Param4<int, ParamsReadContext, PlayerIdentity, Object> RPCCallFunctionParams;