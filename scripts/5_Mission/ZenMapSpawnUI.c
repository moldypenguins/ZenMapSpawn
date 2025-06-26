
class ZenMapSpawnUI extends UIScriptedMenu
{
  protected GridLayout m_SpawnList;
  protected ButtonWidget m_SpawnButton;
  protected int m_SelectedIndex = -1;
  protected ref array<ref ZenMapSpawnPoint> m_SpawnPoints;

  override Widget Init()
  {
    layoutRoot = GetGame().GetWorkspace().CreateWidgets("ZenMapSpawn/gui/layouts/ZenMapSpawnUI.layout");

    m_SpawnList = GridLayout.Cast(layoutRoot.FindAnyWidget("SpawnList"));
    m_SpawnButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("SpawnButton"));

    ZenMapSpawnManager manager = new ZenMapSpawnManager();
    m_SpawnPoints = manager.GetSpawnPoints();

    // Add bed spawn if available
    PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
    if (ZenMapSpawnBedManager.HasBed(player))
    {
      m_SpawnPoints.InsertAt(new ZenMapSpawnPoint("My Bed", ZenMapSpawnBedManager.GetBedSpawnPosition(player)), 0);
    }

    PopulateSpawnList();
    return layoutRoot;
  }

  void PopulateSpawnList()
  {
    for (int i = 0; i < m_SpawnPoints.Count(); i++)
    {
      ZenMapSpawnPoint point = m_SpawnPoints[i];
      ButtonWidget btn = ButtonWidget.Cast(GetGame().GetWorkspace().CreateWidgets("gui/layouts/SpawnListItem.layout", m_SpawnList));
      btn.SetText(point.name);
      btn.SetUserID(i);
    }
  }

  override bool OnClick(Widget w, int x, int y, int button)
  {
    if (w == m_SpawnButton && m_SelectedIndex >= 0)
    {
      ZenMapSpawnPoint selected = m_SpawnPoints[m_SelectedIndex];
      GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(TeleportPlayer, 100, false, selected.position);
      Close();
      return true;
    }

    // Handle spawn list item selection
    ButtonWidget btn = ButtonWidget.Cast(w);
    if (btn && btn.GetUserID() >= 0)
    {
      m_SelectedIndex = btn.GetUserID();
      return true;
    }

    return false;
  }

  void TeleportPlayer(vector pos)
  {
    PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
    if (player)
    {
      player.SetPosition(pos);
    }
  }
}
