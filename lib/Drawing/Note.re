type t = {
  head: Drawable.t(NoteHead.t),
  stem: Drawable.t(Stem.t),
};

let draw = (n, sc) => {
  n.head.draw(sc);
  n.stem.draw(sc);
};

let make = (~clef, ~note, ~x, ()) => {
  let head = NoteHead.make(~clef, ~note, ~x, ());
  let stem = Stem.make(~head=head.content, ());
  let content = {head, stem};
  let draw = draw(content);
  {Drawable.content, draw};
};
