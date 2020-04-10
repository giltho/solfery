type t = {
  glyph: Glyphs.t,
  clef: Solfege.Clef.t,
  scale: float,
  x: float,
};

let draw = (clef, sc) => {
  let paint = Skia.Paint.make();
  Skia.Paint.setTypeface(paint, Glyphs.Font.typeface);
  Skia.Paint.setTextSize(paint, StaffContext.lineSpacing(sc) *. clef.scale);
  Skia.Paint.setColor(paint, Coloring.black);
  let (_, l) = Solfege.Clef.noteOnLine(clef.clef);
  let center = StaffContext.nthLineY(sc, StaffContext.lineOfVisibleLine(l));
  StaffContext.Draw.drawText(~paint, ~x=clef.x, ~y=center, ~text=clef.glyph.text, sc);
  DebugBox.makeAndDraw(~x=clef.x, ~y=center, ~rad=5., sc);
};

let glyphAndScaleOfClef = (clef: Solfege.Clef.t) => {
  switch (clef) {
  | Treble => (Glyphs.gClef, 4.)
  | Bass => (Glyphs.fClef, 4.)
  | Alto => (Glyphs.cClef, 4.)
  };
};

let make = (~clef: Solfege.Clef.t, ~x: float) => {
  let (glyph, scale) = glyphAndScaleOfClef(clef);
  {glyph, clef, x, scale};
};
