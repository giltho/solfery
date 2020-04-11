open Revery;
open Revery.UI;
/* open Revery.UI.Components; */
open Drawing;

/* module Engine = {
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
   }; */

/* module Drawing = {

     let drawLimit =
            (~width: float, ~height: float, canvasContext: CanvasContext.t) => {
          let limit = Engine.limit(~height, ~width);
          let rect = Skia.Rect.makeLtrb(limit, 0., limit +. 1., height);

          let paint = Skia.Paint.make();
          Skia.Paint.setColor(paint, grey);
          CanvasContext.drawRect(~rect, ~paint, canvasContext);
        };
   }; */

let make =
    (
      ~style=Style.emptyViewStyle,
      ~width: float,
      ~height: float,
      ~clef: Solfege.Clef.t,
      /* ~note: Note.t, */
      /* ~children=React.empty, */
      (),
    ) => {
  let makeContext = StaffContext.make(~width, ~height);
  let w_int = int_of_float(width);
  let h_int = int_of_float(height);
  /* let drawNoteAtX = Drawing.drawNoteAtX(~width, ~height, ~clef); */
  /* let drawClef = Drawing.drawClef(~width, ~height, clef); */
  /* let drawLimit = Drawing.drawLimit(~width, ~height); */

  let canvasStyle =
    Style.[width(w_int), height(h_int), backgroundColor(Colors.white)];

  <View style>
    <Canvas
      style=canvasStyle
      render={canvasContext => {
        let staffContext = makeContext(~canvasContext);
        let draw = x => Drawable.draw(staffContext, x);
        draw(StaffLines.make());

        let drawableClef = Clef.make(~clef, ~x=10.);
        draw(drawableClef);

        let note = Solfege.Note.sol(4);
        let note = Note.make(~clef, ~note, ~x=225., ());
        draw(note);
      }}
    />
  </View>;
};
