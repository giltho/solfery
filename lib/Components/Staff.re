open Revery;
open Revery.UI;
/* open Revery.UI.Components; */

let make =
    (
      ~style=Style.emptyViewStyle,
      ~width: float,
      ~height: float,
      ~clef: Solfege.Clef.t,
      /* ~note: Note.t, */
      ~children=React.empty,
      (),
    ) => {
  let makeContext = Drawing.StaffContext.make(~width, ~height);
  let w_int = int_of_float(width);
  let h_int = int_of_float(height);
  /* let drawNoteAtX = Drawing.drawNoteAtX(~width, ~height, ~clef); */
  /* let drawClef = Drawing.drawClef(~width, ~height, clef); */
  /* let drawLimit = Drawing.drawLimit(~width, ~height); */

  let canvasStyle =
    Style.[
      width(w_int),
      height(h_int),
      backgroundColor(Colors.white) ];

  <View style>
    <Canvas
      style=canvasStyle
      render={canvasContext => {
        let staffContext = makeContext(~canvasContext);
        let draw = x => Drawing.Drawable.draw(staffContext, x);

        let drawableClef = Drawing.Clef.make(~clef, ~x=10.);
        draw(drawableClef);
      }}
    >
      { children }
      <StaffLines width height/>
    </Canvas>
  </View>;
};
