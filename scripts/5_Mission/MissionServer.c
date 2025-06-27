// ZenMapSpawnServer.c
// Handles server-side processing of ZenMapSpawn marker RPCs, including cooldown and randomization logic

modded class MissionServer
{
  // This function handles the RPC when a player clicks a spawn marker on the client map.
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

  // Returns a random valid spawn position. Implement actual logic.
  vector GetRandomValidSpawnPoint()
  {
    // TODO: Implement your actual spawn point list logic
    return "7500 0 7500";
  }

  // This function determines if the marker is a valid spawn marker.
  // Implement actual validation logic as needed.
  bool IsValidSpawnMarker(ZenMapMarkerData markerData)
  {
    // Example: Check against known spawn points, or always return true for now
    return true;
  }

  // This function determines if the player is in a spawning state.
  // Customize this logic for your mod/gameplay.
  bool IsPlayerSpawning(PlayerBase player)
  {
    // Example: check if player is dead or in a respawn UI
    return player.IsUnconscious() || player.IsDead() || player.GetHealth("", "") <= 0;
  }
}
