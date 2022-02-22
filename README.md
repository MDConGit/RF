# RPC Framework

Framework for categorizing, logging and manging RPCs to avoid conflicts between mods in DayZ Standalone. This mod is backwards compatible and can act as a drop-in replacement for the RPC management functionality of [Community Framework](https://github.com/Jacob-Mango/DayZ-CommunityFramework "Community Framework"), with a few caveats.

### Why RF vs CF?

The scope of RF is limited **exclusively** to that of RPC handling. It does not provide for any of the additional features of CF, such as notification handling, object removal, animation handling, modstorage, or any of the extra libraries introduced with the update pushed for the launch of DayZ 1.16.

**tldr; anything not included in the RPCManager class is not provided.**

As the scope of CF has increased over time, recent updates have had a negative effect on the stability and reliability of many community servers. Planned features for CF like ModStorage will make removing CF - independent of mod dependency - impossible without wiping the database of community servers.

Reliance on the existence and maintenance of a mod to this extent is distasteful for many server owners, thus the creation of RF.

### Differences

- RF does not handle single player environments gracefully. For single player support, I would reccommend utilizing alternative methods to RPCs for passing data between objects. As the majority of DayZ players spend their time in a multiplayer environment, this was considered a worthwhile tradeoff for simplicity and performance reasons.
- `RPCManager::SendRPCs` is not a provided interface. I couldn't find any examples of it being utilized in popular 3rd party mods, and no documentation existed for it, therefore it was excluded as legacy.

### RF Interfaces

Access to RF should be performed using the `RPCManager` class, which can be accessed globally via `GetRPCManager()` or via `GetGame().GetRPCManager()`. The RPC Manager is instantiated and owned by `DayZGame`.

RPCManager provides the following public methods:

```csharp
bool RegisterRPC(string modName, string functionName, string password, Class targetClass);
```
*Registers an RPC function with the RPCManager class.*

```csharp
void SendSecureRPC(string modName, string functionName, string password, Param params = NULL, bool guaranteed = false, PlayerIdentity recipient = NULL, Object target = NULL);
```
*Sends an RPC to an optional recipient. Omitting or specifying recipient as `NULL` will direct the RPC to the server when executed on client, or to all clients when executed on server*

```csharp
bool SetPassword(string modName, string password);
```
*Single-use function; recursively sets all registered function passwords for `modName` to the specified string.*
```csharp
bool SetPassword(string modName, string functionName, string password);
```
*Overload taking two arguments, will set a specific function's password. Must be performed prior to calling `SetPassword` recursively.*

Additionally, the following legacy methods are provided for compatibility with CF:
```csharp
bool AddRPC(string modName, string functionName, Class targetClass, int executionScope = SingleplayerExecutionType.Server, string password = "");
```
```csharp
void SendRPC(string modName, string functionName, Param params = NULL, bool guaranteed = false, PlayerIdentity recipient = NULL, Object target = NULL);
```

### Logging

RF supports logging of all RPCs passing through the RPCManager, those with `rpc_type == RF_RPC_ID`. A folder will be created in `$profiles` named `RF`.

To enable logging `-dorpclogs 1` must be added as a command line parameter. 

This will log any invalid secure RPCs with missing or invalid passwords. `-dorpclogs 2` may be specified to enable debug logging of all RPCs.