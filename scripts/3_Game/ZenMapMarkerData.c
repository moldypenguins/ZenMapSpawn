class ZenMapMarkerData
{
  vector position;
  string label;
  int color;

  void Init(vector pos, string lbl, int clr)
  {
    position = pos;
    label = lbl;
    color = clr;
  }

  void WriteToContext(ParamsWriteContext ctx)
  {
    ctx.Write(position);
    ctx.Write(label);
    ctx.Write(color);
  }

  bool ReadFromContext(ParamsReadContext ctx)
  {
    if (!ctx.Read(position)) return false;
    if (!ctx.Read(label)) return false;
    if (!ctx.Read(color)) return false;
    return true;
  }
}
