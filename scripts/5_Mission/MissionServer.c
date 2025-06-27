// MissionServer.c
// Handles server-side processing of ZenMapSpawn marker RPCs, including cooldown and randomization logic

modded class MissionServer
{
  // ZenMap spawn manager instance
  ref ZenMapSpawnManager m_ZenMapSpawnManager;

  // Constructor override to initialize ZenMapSpawnManager
  void MissionServer()
  {
    super.MissionServer();
    // Initialize spawn manager
    m_ZenMapSpawnManager = new ZenMapSpawnManager();
  }

  // Handles incoming RPCs from the client, particularly marker click events
  override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
  {
    super.OnRPC(sender, target, rpc_type, ctx);

    if (rpc_type == ZenMapSpawnConstants.RPC_MARKER_CLICK)
    {
      ZenMapMarkerData markerData = new ZenMapMarkerData();
      markerData.ReadFromContext(ctx);

      PlayerBase player = PlayerBase.Cast(target);
      if (!player) return;

      // Check cooldown
      int cooldown = ZenMapSpawnConfig.SelectCooldownSeconds;
      int lastSelect = player.GetVariableInt("ZenMapSpawn_LastSelectTime");
      int now = GetGame().GetTime() / 1000; // seconds

      if (cooldown > 0 && (now - lastSelect) < cooldown)
      {
        // On cooldown: pick a random valid spawn point
        vector randomPos = GetRandomValidSpawnPoint();
        player.SetPosition(randomPos);
        player.SetVariable("ZenMapSpawn_NextSpawnPoint", randomPos, true);
        // Optionally: notify player
        return;
      }

      // Not on cooldown: validate and use picked marker
      if (!IsValidSpawnMarker(markerData))
        return;

      if (IsPlayerSpawning(player))
      {
        player.SetPosition(markerData.pos);
      }
      else
      {
        player.SetVariable("ZenMapSpawn_NextSpawnPoint", markerData.pos, true);
      }
      // Update cooldown timestamp
      player.SetVariable("ZenMapSpawn_LastSelectTime", now, true);
    }
  }

  // Returns a random valid spawn position from loaded spawn points
  vector GetRandomValidSpawnPoint()
  {
    array<ref ZenMapSpawnPoint> points = m_ZenMapSpawnManager.GetSpawnPoints();
    if (points && points.Count() > 0)
    {
      int idx = Math.RandomInt(0, points.Count());
      return points[idx].position;
    }
    // Fallback: center of map
    return "7500 0 7500";
  }

  // Checks if the given marker matches any valid spawn marker
  bool IsValidSpawnMarker(ZenMapMarkerData markerData)
  {
    array<ref ZenMapSpawnPoint> points = m_ZenMapSpawnManager.GetSpawnPoints();
    if (points)
    {
      foreach(ZenMapSpawnPoint spt : points)
      {
        if (vector.Distance(spt.position, markerData.pos) < 1.0)
        {
          return true;
        }
      }
    }
    return false;
  }
}
