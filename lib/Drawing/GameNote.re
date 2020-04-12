type t = {
  note: Drawable.t(Note.t),
  focus: bool,
};

let drawFocus = (xMin, xMax, sc: StaffContext.t) => {
  let color = Coloring.transparentGrey;
  let paint = Skia.Paint.make();
  Skia.Paint.setColor(paint, color);
  let rect = Skia.Rect.makeLtrb(xMin, 0., xMax, StaffContext.height(sc));
  StaffContext.Draw.drawRect(~paint, ~rect, sc);
};

let draw = (gn: t, sc) => {
  if (gn.focus) {
    let head = gn.note.content.head.content;
    let headGlyphName = head.glyph.name;
    let ls = StaffContext.lineSpacing(sc);
    let headX = head.x;
    let lBox = Glyphs.Coord.getBBoxSW(headGlyphName).x;
    let rBox = Glyphs.Coord.getBBoxNE(headGlyphName).x;
    let xMin = headX +. lBox *. ls -. 3.;
    let xMax = headX +. rBox *. ls +. 3.;
    drawFocus(xMin, xMax, sc);
  };
  gn.note.draw(sc);
};

let make = (~clef, ~note, ~x, ~color=Coloring.black, ~focus=false, ()) => {
  let note = Note.make(~clef, ~note, ~x, ~color, ());
  let content = {note, focus};
  let draw = draw(content);
  {Drawable.content, draw};
};
