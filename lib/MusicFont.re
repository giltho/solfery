let fontFile = "Metdemo.ttf";

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
  bottomFromCenter: float
};

let treble = {str: ":", factor: 4.1, leftFromCenter: 0.42, bottomFromCenter: 0.12};

let bass = {str: "$", factor: 4.1, leftFromCenter: 0.42, bottomFromCenter: 0.12};

let quarter = {str: "Q", factor: 4.2, leftFromCenter: 0.2, bottomFromCenter: 0.088};

let ofClef = (clef: Solfege.Clef.t) => {
  switch (clef) {
  | Treble => treble
  | Bass => bass
  | Alto => failwith("Not supporting alto key yet")
  };
};

let howToDrawAround = (~layerSize: float, ~fontData: t, x, y) => {
  let fontSize = layerSize *. fontData.factor;
  let x = x -. (fontData.leftFromCenter *. fontSize);
  let y = y +. (fontData.bottomFromCenter *. fontSize);
  (fontSize, x, y);
};
