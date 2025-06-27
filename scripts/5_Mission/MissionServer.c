modded class MissionServer
{
  override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
  {
    super.OnRPC(sender, target, rpc_type, ctx);

    if (rpc_type == ZenMapConstants.ZEN_MAP_RPC_MARKER_CLICK)
    {
      ZenMapMarkerData data = new ZenMapMarkerData();
      if (data.ReadFromContext(ctx))
      {
        Print("[ZenMap] Received marker RPC:");
        Print("  Position: " + data.position);
        Print("  Label: " + data.label);
        Print("  Color: " + data.color);
        // TODO: handle marker logic here (broadcast to other players, log, etc.)
      }
      else
      {
        Print("[ZenMap] Failed to deserialize marker data from RPC!");
      }
    }
  }
}
