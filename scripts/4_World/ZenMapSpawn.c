// ZenMapSpawn.c
// Handles map spawn logic and marker display using ZenMap API

ref ZenMapSpawnManager g_ZenMapSpawnManager; // Global manager instance

// ZenMapSpawnPoint class for map spawn points (use ZenMapSpawnPoint from ZenMapSpawnManager)
class ZenMapSpawnPoint
{
  vector Position;
  string Name;
  string IconPath;

  // Constructor for ZenMapSpawnPoint
  void ZenMapSpawnPoint(vector pos, string name, string iconPath)
  {
    Position = pos;
    Name = name;
    IconPath = iconPath;
  }
}

// Loads spawn points using ZenMapSpawnManager
void ZenMapSpawn_LoadSpawnPoints()
{
  if (!g_ZenMapSpawnManager) {
    g_ZenMapSpawnManager = new ZenMapSpawnManager();
  }
}

// Shows map and adds spawn markers for the player
void ZenMapSpawn_ShowMapAndMarkers(PlayerBase player)
{
  auto zenMapPlugin = GetZenMapPlugin();
  if (zenMapPlugin && g_ZenMapSpawnManager)
  {
    zenMapPlugin.ShowMapForPlayer(player, true);
    array<ref ZenMapSpawnPoint> points = g_ZenMapSpawnManager.GetSpawnPoints();
    foreach(ZenMapSpawnPoint spt : points)
    {
      string markerName = "ZenMapSpawn_" + spt.Name;
      int color = -65536;
      int iconIdx = GetZenMapConfig().FileToArrayIndex(spt.IconPath);
      zenMapPlugin.AddMarker(player, new MapMarker(spt.Position, markerName, color, iconIdx));
    }
  }
}

// Removes all spawn markers for the player
void ZenMapSpawn_RemoveAllSpawnMarkers(PlayerBase player)
{
  auto zenMapPlugin = GetZenMapPlugin();
  if (!zenMapPlugin || !g_ZenMapSpawnManager) return;
  array<ref ZenMapSpawnPoint> points = g_ZenMapSpawnManager.GetSpawnPoints();
  foreach(ZenMapSpawnPoint spt : points)
  {
    string markerName = "ZenMapSpawn_" + spt.Name;
    zenMapPlugin.RemoveMarkerByName(player, markerName);
  }
}

// Handles marker click events
void ZenMapSpawn_OnMarkerClicked(vector markerPos, string markerLabel, int markerColor)
{
  ZenMapMarkerData data = new ZenMapMarkerData();
  data.Init(markerPos, markerLabel, markerColor);

  ScriptRPC rpc = new ScriptRPC();
  data.WriteToContext(rpc);
  GetGame().RPCSingleParam(
    null,
    ZenMapSpawnConstants.RPC_MARKER_CLICK,
    rpc,
    true,
    null
  );
}

// Mission hooks for CustomMission
modded class CustomMission
{
  // Initializes ZenMap plugin and hooks marker clicked event
  override void OnInit()
  {
    super.OnInit();
    auto zenMapPlugin = GetZenMapPlugin();
    if (zenMapPlugin)
    {
      zenMapPlugin.OnMarkerClicked.Insert(ZenMapSpawn_OnMarkerClicked);
    }
  }

  // Loads spawn points and shows map on player respawn
  override void OnClientRespawnEvent(PlayerBase player, int respawnType)
  {
    ZenMapSpawn_LoadSpawnPoints();
    ZenMapSpawn_ShowMapAndMarkers(player);
  }
}
