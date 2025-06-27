// ZenMapSpawnUI.c
// Handles client-side map marker click and sends RPC to the server for ZenMapSpawn

// This is an example UI or manager class for handling marker clicks
class ZenMapSpawnUI
{
  // Call this from your actual UI event/callback when a marker is clicked
  void OnMapMarkerClicked(vector markerPos, string markerLabel, int markerColor)
  {
    ZenMapMarkerData data = new ZenMapMarkerData();
    data.Init(markerPos, markerLabel, markerColor);

    ScriptRPC rpc = new ScriptRPC();
    data.WriteToContext(rpc);
    // Send to server
    GetGame().RPCSingleParam(null, ZenMapSpawnConstants.RPC_MARKER_CLICK, rpc, true, null);
  }
}

// Example usage (replace this with your actual UI hookup)
// This part would typically be connected to your map UI logic
void ZenMapMarkerClick()
{
  ZenMapSpawnUI ui = new ZenMapSpawnUI();
  vector pos = "1234 0 5678";
  string label = "My Marker";
  int color = 0xFF00FF00; // ARGB green
  ui.OnMapMarkerClicked(pos, label, color);
}
