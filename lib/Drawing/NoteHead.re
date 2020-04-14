open Solfege;
open Revery.Draw;

type t = {
  glyph: Glyphs.t,
  note: Solfege.Note.t,
  clef: Clef.t,
  scale: float,
  x: float,
  color: Skia.Color.t,
  opacity: float,
};

let shouldDrawLineUntil = (notehead: t) => {
  let (refNote, refLine) = Clef.noteOnLine(notehead.clef);
  let distFromRef = Solfege.Note.compare(notehead.note, refNote);
  let line = refLine + distFromRef / 2;
  if (line < 0) {
    `Up(line);
  } else if (line > 4) {
    `Down(line);
  } else {
    `Dont;
  };
};

let lineRect = (notehead: t, n: int, sc: StaffContext.t) => {
  let {glyph, x, _} = notehead;
  let y = StaffContext.(nthLineY(sc, lineOfVisibleLineUnsafe(n)));
  let (xMin, _, xMax, _) = StaffContext.glyphBoxRect(~glyph, ~x, ~y, sc);
  let width = xMax -. xMin;
  let overshoot = (StaffContext.lineHeight(sc) -. 1.) /. 2.;
  let yMin = y -. overshoot;
  let yMax = y +. overshoot;
  Skia.Rect.makeLtrb(xMin -. width /. 3., yMin, xMax +. width /. 3., yMax);
};

let drawLines = (notehead, sc) => {
  let paint = Skia.Paint.make();
  Skia.Paint.setColor(paint, notehead.color);
  Skia.Paint.setAlpha(paint, notehead.opacity);
  switch (shouldDrawLineUntil(notehead)) {
  | `Dont => ()
  | `Up(l) =>
    for (n in l to (-1)) {
      let rect = lineRect(notehead, n, sc);
      StaffContext.Draw.drawRect(~paint, ~rect, sc);
    }
  | `Down(l) =>
    for (n in 5 to l) {
      let rect = lineRect(notehead, n, sc);
      StaffContext.Draw.drawRect(~paint, ~rect, sc);
    }
  };
};

let centerY = (notehead: t, sc: StaffContext.t) => {
  let (refNote, refLine) = Clef.noteOnLine(notehead.clef);
  let distFromRef = Solfege.Note.compare(notehead.note, refNote);
  let refLine = StaffContext.lineOfVisibleLine(refLine);
  let halfLineSpace = StaffContext.lineSpacing(sc) /. 2.;
  halfLineSpace
  *. float_of_int(distFromRef)
  +. StaffContext.nthLineY(sc, refLine);
};

let draw = (notehead, sc) => {
  drawLines(notehead, sc);
  let paint = Skia.Paint.make();
  Skia.Paint.setTypeface(paint, Glyphs.Font.typeface);
  Skia.Paint.setTextSize(
    paint,
    StaffContext.lineSpacing(sc) *. notehead.scale,
  );
  Skia.Paint.setColor(paint, notehead.color);
  Skia.Paint.setAlpha(paint, notehead.opacity);
  let y = centerY(notehead, sc);
  StaffContext.Draw.drawText(
    ~paint,
    ~x=notehead.x,
    ~y,
    ~text=notehead.glyph.text,
    sc,
  );
};

let make = (~clef, ~note, ~x, ~color=Coloring.black, ~opacity=1., ()) => {
  let scale = 4.;
  let content = {
    glyph: Glyphs.noteheadBlack,
    note,
    clef,
    x,
    scale,
    opacity,
    color,
  };
  let draw = draw(content);
  {Drawable.content, draw};
};
