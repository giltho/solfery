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

let shouldDrawLine = (notehead: t) => {
  let (refNote, refLine) = Clef.noteOnLine(notehead.clef);
  let distFromRef = Solfege.Note.compare(notehead.note, refNote);
  distFromRef
  mod 2 === 0
  && {
    let line = refLine + distFromRef / 2;
    line < 0 || line > 4;
  };
};

let lineRect = (notehead: t, y: float, sc: StaffContext.t) => {
  let { glyph, x, _ } = notehead;
  let (xMin, _, xMax, _) = StaffContext.glyphBoxRect(~glyph, ~x, ~y=0., sc);
  let width = xMax -. xMin;
  let overshoot = (StaffContext.lineHeight(sc) -. 1.) /. 2.;
  let yMin = y -. overshoot;
  let yMax = y +. overshoot;
  Skia.Rect.makeLtrb(
    xMin -. (width /. 2.),
    yMin,
    xMax +. (width /. 2.),
    yMax
  )
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
