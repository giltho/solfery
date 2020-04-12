open Solfege;
open Revery.Draw;

type t = {
  glyph: Glyphs.t,
  note: Solfege.Note.t,
  clef: Clef.t,
  scale: float,
  x: float,
  color: Skia.Color.t,
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
  let paint = Skia.Paint.make();
  Skia.Paint.setTypeface(paint, Glyphs.Font.typeface);
  Skia.Paint.setTextSize(
    paint,
    StaffContext.lineSpacing(sc) *. notehead.scale,
  );
  Skia.Paint.setColor(paint, notehead.color);
  let y = centerY(notehead, sc);
  StaffContext.Draw.drawText(
    ~paint,
    ~x=notehead.x,
    ~y,
    ~text=notehead.glyph.text,
    sc,
  );
};

let make = (~clef, ~note, ~x, ~color=Coloring.black, ()) => {
  let scale = 4.;
  let content = {glyph: Glyphs.noteheadBlack, note, clef, x, scale, color};
  let draw = draw(content);
  {Drawable.content, draw};
};
