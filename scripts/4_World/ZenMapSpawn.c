// ZenMapSpawn.c
// Show map and add spawn markers using ZenMap API

static ref array<ref ZenMapSpawnPoint> g_ZenMapSpawnPoints;

class ZenMapSpawnPoint
{
  vector Position;
  string Name;
  string IconPath;

  void ZenMapSpawnPoint(vector pos, string name, string iconPath)
  {
    Position = pos;
    Name = name;
    IconPath = iconPath;
  }
}

void ZenMapSpawn_LoadSpawnPoints()
{
  if (!g_ZenMapSpawnPoints) g_ZenMapSpawnPoints = new array<ref ZenMapSpawnPoint>;
  g_ZenMapSpawnPoints.Clear();
  g_ZenMapSpawnPoints.Insert(new ZenMapSpawnPoint("8229.1 471.2 9531.2", "Cherno", "ZenMap/data/icons/heli.paa"));
  g_ZenMapSpawnPoints.Insert(new ZenMapSpawnPoint("1200 0 3500", "Kamenka", "ZenMap/data/icons/heli.paa"));
  g_ZenMapSpawnPoints.Insert(new ZenMapSpawnPoint("4300 0 8000", "Vybor", "ZenMap/data/icons/heli.paa"));
}

modded class CustomMission
{
  override void OnInit()
  {
    super.OnInit();
    auto zenMapPlugin = GetZenMapPlugin();
    if (zenMapPlugin)
    {
      zenMapPlugin.OnMarkerClicked.Insert(ZenMapSpawn_OnMarkerClicked);
    }
  }

  override void OnClientRespawnEvent(PlayerBase player, int respawnType)
  {
    ZenMapSpawn_LoadSpawnPoints();
    ZenMapSpawn_ShowMapAndMarkers(player);
  }
}

void ZenMapSpawn_ShowMapAndMarkers(PlayerBase player)
{
  auto zenMapPlugin = GetZenMapPlugin();
  if (zenMapPlugin)
  {
    zenMapPlugin.ShowMapForPlayer(player, true);
    foreach(ZenMapSpawnPoint spt : g_ZenMapSpawnPoints)
    {
      string markerName = "ZenMapSpawn_" + spt.Name;
      int color = -65536;
      int iconIdx = GetZenMapConfig().FileToArrayIndex(spt.IconPath);
      zenMapPlugin.AddMarker(player, new MapMarker(spt.Position, markerName, color, iconIdx));
    }
  }
}

void ZenMapSpawn_RemoveAllSpawnMarkers(PlayerBase player)
{
  auto zenMapPlugin = GetZenMapPlugin();
  if (!zenMapPlugin) return;
  foreach(ZenMapSpawnPoint spt : g_ZenMapSpawnPoints)
  {
    string markerName = "ZenMapSpawn_" + spt.Name;
    zenMapPlugin.RemoveMarkerByName(player, markerName);
  }
}

void ZenMapSpawn_OnMarkerClicked(vector markerPos, string markerLabel, int markerColor)
{
  ZenMapMarkerData data = new ZenMapMarkerData();
  data.Init(markerPos, markerLabel, markerColor);

  ScriptRPC rpc = new ScriptRPC();
  data.WriteToContext(rpc);
  GetGame().RPCSingleParam(
    null,
    ZenMapConstants.ZEN_MAP_RPC_MARKER_CLICK,
    rpc,
    true,
    null
  );
}
