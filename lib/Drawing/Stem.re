type t = {
  head: NoteHead.t,
  direction: [ | `Up | `Down],
  color: Skia.Color.t,
  opacity: float,
};

let stemWidth = sc => {
  /* For 200px staff height, it should be 2 pixels wide */
  StaffContext.height(sc) /. 100.;
};

let rectOfStemDown = (stem, x, y, sc) => {
  let nw = Glyphs.Coord.getStemDownNW(stem.head.glyph.name);
  let lsp = StaffContext.lineSpacing(sc);
  let l = nw.x *. lsp;
  let r = l +. stemWidth(sc);
  let t = -. nw.y *. lsp;
  let b = t +. 3.5 *. lsp +. StaffContext.lineHeight(sc);
  Skia.Rect.makeLtrb(x +. l, y +. t, x +. r, y +. b);
};

let rectOfStemUp = (stem, x, y, sc) => {
  let se = Glyphs.Coord.getStemUpSE(stem.head.glyph.name);
  let lsp = StaffContext.lineSpacing(sc);
  let r = se.x *. lsp;
  let l = r -. stemWidth(sc);
  let b = -. se.y *. lsp;
  let t = b -. 3.5 *. lsp +. StaffContext.lineHeight(sc);
  Skia.Rect.makeLtrb(x +. l, y +. t, x +. r, y +. b);
};

let draw = (stem: t, sc: StaffContext.t) => {
  /* let scale = StaffContext.lineSpacing(sc) +. 2. *. sc.lineHeight; */
  let x = stem.head.x;
  let y = NoteHead.centerY(stem.head, sc);
  let rect =
    switch (stem.direction) {
    | `Up => rectOfStemUp(stem, x, y, sc)
    | `Down => rectOfStemDown(stem, x, y, sc)
    };
  let paint = Skia.Paint.make();
  Skia.Paint.setColor(paint, stem.color);
  Skia.Paint.setAlpha(paint, stem.opacity);
  StaffContext.Draw.drawRect(~rect, ~paint, sc);
};

let make = (~head: NoteHead.t, ~color=Coloring.black, ~opacity=1., ()) => {
  let direction = `Up;
  /** We'll decide later */
  let content = {head, direction, color, opacity};
  {Drawable.content, draw: draw(content)};
};
