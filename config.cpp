class CfgPatches
{
  class ZenMapSpawn
  {
    units[] = {};
    weapons[] = {};
    requiredVersion = 0.1;
    requiredAddons[] = {};
  };
};

class CfgMods
{
  class ZenMapSpawn
  {
    dir = "ZenMapSpawn";
    picture = "";
    action = "";
    hideName = 1;
    hidePicture = 1;
    name = "ZenMapSpawn";
    credits = "";
    author = "moldypenguins";
    version = "1.0";
    type = "mod";
    dependencies[] = {"Game", "World", "Mission"};

    class defs
    {
      class coreScriptModule
      {
        value = "";
        files[] = {"ZenMapSpawn/scripts/1_Core"};
      };
      class gameScriptModule
      {
        value = "";
        files[] = {"ZenMapSpawn/scripts/3_Game"};
      };
      class worldScriptModule
      {
        value = "";
        files[] = {"ZenMapSpawn/scripts/4_World"};
      };
      class missionScriptModule
      {
        value = "";
        files[] = {"ZenMapSpawn/scripts/5_Mission"};
      };
    };
  };
};
