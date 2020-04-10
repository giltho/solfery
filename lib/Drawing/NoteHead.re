open Solfege;

type t = {
  glyph: Glyphs.t,
  note: Note.t,
  clef: Clef.t,
  x: float,
};

let draw = (_notehead, _cs) => ();

let make = (~clef, ~note, ~x, ()) => {
  let content = {glyph: Glyphs.noteheadBlack, note, clef, x};
  let draw = draw(content);
  {Drawable.content, draw};
};
