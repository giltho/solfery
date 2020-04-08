open Revery;
open Revery.UI;
/* open Revery.UI.Components; */
open Revery.Draw;
open Solfege;

let black = Skia.Color.makeArgb(0xFFl, 0x00l, 0x00l, 0x00l);
let blue = Skia.Color.makeArgb(0xFFl, 0x00l, 0x00l, 0xFFl);
let red = Skia.Color.makeArgb(0xFFl, 0xFFl, 0x00l, 0x00l);

module Engine = {
  /**
   * # Engine
   *
   * This module contains the eniine for printing a Staff.
   * A staff can display 21 different notes
   * (we go up to 3 lines above and 3 lines under the standard 5 lines)
   *
   * Accordingly, the view is split into 12 layers (between the lines, including invisible ones).
   *
   * */

  let nLayers = 12;

  /**
   * Number of pixels on that the line should take vertically
   */
  let lineHeight = 2.;

  let layerSize = (height: float) => height /. float_of_int(nLayers);

  /**
    * The first layer is of index 0
    */
  let nthLayerStartY = (~height: float, n: int) =>
    float_of_int(n) *. layerSize(height);

  /**
   * Returns the number of the layer at the begining of which the visible line is.
   * Beginning means (the line is "above" this layer)
   */
  let layerOfVisibleLine = (n: int) => {
    if (n < 0 || n > 4) {
      raise(
        Invalid_argument(
          "There are only 5 visible lines, cannot look for the "
          ++ string_of_int(n + 1)
          ++ "nth.",
        ),
      );
    };
    n + 4;
  };

  let rectOfVisibleLine = (~width: float, ~height: float, n: int) => {
    let halfLineHeight = lineHeight /. 2.;
    let nLayer = layerOfVisibleLine(n);
    let layerYStart = layerSize(height) *. float_of_int(nLayer);
    let yStart = layerYStart -. halfLineHeight;
    let yEnd = yStart +. lineHeight;
    let xStart = 0.;
    let xEnd = width;
    (xStart, yStart, xEnd, yEnd);
  };

  let noteCenterY = (~height: float, ~clef: Clef.t, note: Note.t) => {
    let (refNote, refLine) = Clef.noteOnLine(clef);
    let distFromRef = Note.compare(note, refNote);
    let refLayer = layerOfVisibleLine(refLine);
    let halfLayerSize = layerSize(height) /. 2.;
    halfLayerSize
    *. float_of_int(distFromRef)
    +. nthLayerStartY(~height, refLayer);
  };
};

let noteCircle = (~width: float, ~height: float, ~clef: Clef.t, note: Note.t) => {
  let noteCenterX = width /. 2.;
  let noteCenterY = Engine.noteCenterY(~height, ~clef, note);
  let noteRadius = Engine.layerSize(height) /. 2.;
  (noteCenterX, noteCenterY, noteRadius);
};

let drawClef =
  (
    ~width: float,
    ~height: float,
    clef: Clef.t,
    canvasContext: CanvasContext.t
  ) => {
    let x0 = 100.;
    let (_, y0, r) = noteCircle(~width, ~height, ~clef, Note.sol(4));

    let clefFont = MusicFont.ofClef(clef);

    let (fontSize, x, y) =
      MusicFont.howToDrawAround(
        Engine.layerSize(height),
        clefFont,
        x0,
        y0
      );

    let paint = Skia.Paint.make();
    Skia.Paint.setColor(paint, black);
    let tf = MusicFont.typeface;
    Skia.Paint.setTypeface(paint, tf);
    Skia.Paint.setTextSize(paint, fontSize);
    CanvasContext.drawText(~paint, ~x, ~y, ~text=clefFont.str, canvasContext);

    let placeHolderFill = Skia.Paint.make();
    Skia.Paint.setColor(placeHolderFill, red);
    CanvasContext.drawRectLtwh(
      ~top=y0 -. r /. 2.,
      ~left=x0 -. r /. 2.,
      ~width=r,
      ~height=r,
      ~paint=placeHolderFill,
      canvasContext,
    );
  };

let drawNote =
    (
      ~width: float,
      ~height: float,
      ~clef: Clef.t,
      canvasContext: CanvasContext.t,
      note: Note.t,
    ) => {
  let (x0, y0, r) = noteCircle(~width, ~height, ~clef, note);
  let (fontSize, x, y) =
    MusicFont.howToDrawAround(
      Engine.layerSize(height),
      MusicFont.quarter,
      x0,
      y0,
    );
  let paint = Skia.Paint.make();
  Skia.Paint.setColor(paint, black);
  let tf = MusicFont.typeface;
  Skia.Paint.setTypeface(paint, tf);
  Skia.Paint.setTextSize(paint, fontSize);
  CanvasContext.drawText(~paint, ~x, ~y, ~text=MusicFont.quarter.str, canvasContext);

  let placeHolderFill = Skia.Paint.make();
  Skia.Paint.setColor(placeHolderFill, red);
  CanvasContext.drawRectLtwh(
    ~top=y0 -. r /. 2.,
    ~left=x0 -. r /. 2.,
    ~width=r,
    ~height=r,
    ~paint=placeHolderFill,
    canvasContext,
  );
};

let nthLineRect = (~width: float, ~height: float, n: int) => {
  let (a, b, c, d) = Engine.rectOfVisibleLine(~width, ~height, n);
  Skia.Rect.makeLtrb(a, b, c, d);
};

let drawVisibleLines =
    (~width: float, ~height: float, canvasContext: CanvasContext.t) => {
  let paint = Skia.Paint.make();
  Skia.Paint.setColor(paint, black);

  for (n in 0 to 4) {
    let rect = nthLineRect(~width, ~height, n);
    CanvasContext.drawRect(~rect, ~paint, canvasContext);
  };
};

let make = (~width: float, ~height: float, ~clef: Clef.t, ~note: Note.t, ()) => {
  let w_int = int_of_float(width);
  let h_int = int_of_float(height);
  let drawVisibleLines = drawVisibleLines(~width, ~height);
  let drawNote = drawNote(~width, ~height, ~clef);
  let drawClef = drawClef(~width, ~height, clef);

  let canvas_style =
    Style.[width(w_int), height(h_int), backgroundColor(Colors.white)];

  <View>
    <Canvas
      style=canvas_style
      render={canvasContext => {
        drawVisibleLines(canvasContext);
        drawClef(canvasContext);
        drawNote(canvasContext, note);
      }}
    />
  </View>;
};
