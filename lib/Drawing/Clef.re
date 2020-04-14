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
  let y = StaffContext.nthLineY(sc, StaffContext.lineOfVisibleLine(l));
  StaffContext.Draw.drawText(
    ~paint,
    ~x=clef.x,
    ~y,
    ~text=clef.glyph.text,
    sc,
  );
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
  let content = {glyph, clef, x, scale};
  Drawable.make (content, draw);
};
