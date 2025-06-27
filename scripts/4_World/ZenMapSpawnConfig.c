// File: scripts/4_world/config/ZenMapSpawnConfig.c

class ZenMapSpawnConfig : Managed
{
  private const static string zenModFolder = "$profile:\\Zenarchist\\";
  private const static string zenConfigName = "ZenMapSpawnConfig.json";

  // Bed respawning enabled (true) or disabled (false)
  bool BedRespawningEnabled = true;

  // Cooldown in seconds for spawn selection (0 = disabled)
  int SpawnSelectCooldownSeconds = 60;

  void Load()
  {
    if (GetGame().IsDedicatedServer())
      return;

    if (FileExist(zenModFolder + zenConfigName))
    {
      JsonFileLoader<ZenMapSpawnConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);
      return;
    }

    Save();
  }

  void Save()
  {
    // If config folder doesn't exist, create it.
    if (!FileExist(zenModFolder))
    {
      MakeDirectory(zenModFolder);
    }

    string savePath = zenModFolder + zenConfigName;
    Print("[ZenMapSpawn] Saving config to " + savePath);

    JsonFileLoader<ZenMapSpawnConfig>.JsonSaveFile(savePath, this);
  }
}

static ref ZenMapSpawnConfig m_ZenMapSpawnConfig;

static ZenMapSpawnConfig GetZenMapSpawnConfig()
{
  if (!m_ZenMapSpawnConfig)
  {
    Print("[ZenMapSpawnConfig] Init");
    m_ZenMapSpawnConfig = new ZenMapSpawnConfig;
    m_ZenMapSpawnConfig.Load();
  }

  return m_ZenMapSpawnConfig;
}

// Helper function to verify config is loaded
static bool IsZenMapSpawnConfigLoaded()
{
  return m_ZenMapSpawnConfig != null;
}
