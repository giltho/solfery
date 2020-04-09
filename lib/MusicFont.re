let fontFile = "Bravura.otf";

let font =
  switch (Revery.Font.load(fontFile)) {
  | Ok(f) => f
  | Error(m) => failwith(m)
  };

let typeface = Revery.Font.getSkiaTypeface(font);

type t = {
  str: string,
  factor: float,
  leftFromCenter: float,
  bottomFromCenter: float,
  width: float,
};

let treble = {
  str: {||},
  factor: 4.1,
  leftFromCenter: 0.42,
  bottomFromCenter: 0.12,
  width: 0.7 /* this is very approximate */
};

let bass = {
  str: {||},
  factor: 4.1,
  leftFromCenter: 0.13,
  bottomFromCenter: 0.62,
  width: 0.4,
};

let alto = {
  str: {||},
  factor: 4.1,
  leftFromCenter: 0.13,
  bottomFromCenter: 0.62,
  width: 0.4
};

let quarter = {
  str: {||},
  factor: 4.2,
  leftFromCenter: 0.2,
  bottomFromCenter: 0.088,
  width: 0.36,
};

let ofClef = (clef: Solfege.Clef.t) => {
  switch (clef) {
  | Treble => treble
  | Bass => bass
  | Alto => alto
  };
};

let fontSize = (~layerSize: float, ~fontData: t) =>
  layerSize *. fontData.factor;

let howToDrawAround = (~layerSize: float, ~fontData: t, x, y) => {
  let fontSize = fontSize(~layerSize, ~fontData);
  let x = x -. fontData.leftFromCenter *. fontSize;
  let y = y +. fontData.bottomFromCenter *. fontSize;
  (fontSize, x, y);
};
