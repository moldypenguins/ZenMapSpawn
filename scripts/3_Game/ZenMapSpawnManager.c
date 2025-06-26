
class ZenMapSpawnManager
{
  ref array<ref ZenMapSpawnPoint> m_SpawnPoints;

  void ZenMapSpawnManager()
  {
    m_SpawnPoints = new array<ref ZenMapSpawnPoint>();
    LoadSpawnPoints();
  }

  void LoadSpawnPoints()
  {
    string missionFolder = GetMissionPath();
    string filePath = missionFolder + "cfgplayerspawnpoints.xml";

    XmlParser xml = new XmlParser();
    XmlDocument doc;
    if (!xml.ParseFile(filePath, doc))
    {
      Print("[ZenMapSpawn] Failed to load spawn points from: " + filePath);
      return;
    }

    XmlElement root = doc.GetRootElement();
    for (int i = 0; i < root.GetChildrenCount(); i++)
    {
      XmlElement point = root.GetChild(i);
      string name = point.GetAttribute("name");
      float x = point.GetAttribute("x").ToFloat();
      float y = point.GetAttribute("y").ToFloat();
      float z = point.GetAttribute("z").ToFloat();
      m_SpawnPoints.Insert(new ZenMapSpawnPoint(name, Vector(x, y, z)));
    }

    Print("[ZenMapSpawn] Loaded " + m_SpawnPoints.Count() + " spawn points.");
  }

  array<ref ZenMapSpawnPoint> GetSpawnPoints()
  {
    return m_SpawnPoints;
  }
}
