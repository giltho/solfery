type t = {
  head: Drawable.t(NoteHead.t),
  stem: Drawable.t(Stem.t),
};

let draw = (n, sc) => {
  n.head.draw(sc);
  n.stem.draw(sc);
};

let make = (~clef, ~note, ~x, ~color=Coloring.black, ~opacity=1., ()) => {
  let head = NoteHead.make(~clef, ~note, ~x, ~color, ~opacity, ());
  let stem = Stem.make(~head=head.content, ~color, ~opacity, ());
  let content = {head, stem};
  let draw = draw(content);
  {Drawable.content, draw};
};
