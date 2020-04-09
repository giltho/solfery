open Revery;
open Revery.UI;
/* open Revery.UI.Components; */
open Revery.Draw;
open Solfege;

let black = Skia.Color.makeArgb(0xFFl, 0x00l, 0x00l, 0x00l);
let blue = Skia.Color.makeArgb(0xFFl, 0x00l, 0x00l, 0xFFl);
let red = Skia.Color.makeArgb(0xFFl, 0xFFl, 0x00l, 0x00l);
let grey = Skia.Color.makeArgb(0x30l, 0x00l, 0x00l, 0x00l);

module DisplayableNote = {
  type t = {
    note: Note.t,
    focus: bool,
  };

  let make = (~focus=false, ~note: Note.t, ()) => {
    {note, focus};
  };
};

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

  let limit = (~height: float, ~width: float) => {
    /* We take the treble key as a reference to make sure everything is aligned */
    let sz =
      MusicFont.fontSize(
        ~layerSize=layerSize(height),
        ~fontData=MusicFont.treble,
      );
    let width = MusicFont.treble.width *. sz;
    width *. 4.;
  };
};

module Drawing = {
  let drawSymb =
      (
        ~height: float,
        ~fontData: MusicFont.t,
        ~focus=false,
        x0: float,
        y0: float,
        canvasContext: CanvasContext.t,
      ) => {
    let (fontSize, x, y) =
      MusicFont.howToDrawAround(Engine.layerSize(height), fontData, x0, y0);
    let paint = Skia.Paint.make();

    if (focus) {
      let paint = Skia.Paint.make();
      Skia.Paint.setColor(paint, grey);

      let fw = fontSize *. fontData.width;

      CanvasContext.drawRectLtwh(
        ~left=x0 -. 0.6 *. fw,
        ~top=0.,
        ~width=fw *. 1.2,
        ~height,
        ~paint,
        canvasContext,
      );
    };

    Skia.Paint.setColor(paint, black);
    let tf = MusicFont.typeface;
    Skia.Paint.setTypeface(paint, tf);
    Skia.Paint.setTextSize(paint, fontSize);
    CanvasContext.drawText(~paint, ~x, ~y, ~text=fontData.str, canvasContext);
  };

  let drawClef =
      (
        ~width: float,
        ~height: float,
        clef: Clef.t,
        canvasContext: CanvasContext.t,
      ) => {
    let (correspondingNote, _) = Clef.noteOnLine(clef);
    let y0 = Engine.noteCenterY(~height, ~clef, correspondingNote);
    let clefFont = MusicFont.ofClef(clef);
    let x0 =
      clefFont.width
      *. MusicFont.fontSize(
           ~layerSize=Engine.layerSize(height),
           ~fontData=clefFont,
         );
    drawSymb(~height, ~fontData=clefFont, x0, y0, canvasContext);
  };

  let drawNoteAtX =
      (
        ~width: float,
        ~height: float,
        ~clef: Clef.t,
        canvasContext: CanvasContext.t,
        note: DisplayableNote.t,
        x0: float,
      ) => {
    let y0 = Engine.noteCenterY(~height, ~clef, note.note);
    drawSymb(
      ~height,
      ~fontData=MusicFont.quarter,
      ~focus=note.focus,
      x0,
      y0,
      canvasContext,
    );
  };

  let drawVisibleLines =
      (~width: float, ~height: float, canvasContext: CanvasContext.t) => {
    let nthLineRect = (~width: float, ~height: float, n: int) => {
      let (a, b, c, d) = Engine.rectOfVisibleLine(~width, ~height, n);
      Skia.Rect.makeLtrb(a, b, c, d);
    };

    let paint = Skia.Paint.make();
    Skia.Paint.setColor(paint, black);

    for (n in 0 to 4) {
      let rect = nthLineRect(~width, ~height, n);
      CanvasContext.drawRect(~rect, ~paint, canvasContext);
    };
  };

  let drawLimit =
      (~width: float, ~height: float, canvasContext: CanvasContext.t) => {
    let limit = Engine.limit(~height, ~width);
    let rect = Skia.Rect.makeLtrb(limit, 0., limit +. 1., height);

    let paint = Skia.Paint.make();
    Skia.Paint.setColor(paint, grey);
    CanvasContext.drawRect(~rect, ~paint, canvasContext);
  };
};

let make =
    (
      ~style=Style.emptyViewStyle,
      ~width: float,
      ~height: float,
      ~clef: Clef.t,
      ~note: Note.t,
      (),
    ) => {
  let w_int = int_of_float(width);
  let h_int = int_of_float(height);
  let drawVisibleLines = Drawing.drawVisibleLines(~width, ~height);
  let drawNoteAtX = Drawing.drawNoteAtX(~width, ~height, ~clef);
  let drawClef = Drawing.drawClef(~width, ~height, clef);
  let drawLimit = Drawing.drawLimit(~width, ~height);

  let canvasStyle =
    Style.[width(w_int), height(h_int), backgroundColor(Colors.white)];

  <View style>
    <Canvas
      style=canvasStyle
      render={canvasContext => {
        drawVisibleLines(canvasContext);
        drawClef(canvasContext);
        drawLimit(canvasContext);
        drawNoteAtX(
          canvasContext,
          DisplayableNote.make(~focus=false, ~note, ()),
          width /. 2.,
        );
      }}
    />
  </View>;
};
