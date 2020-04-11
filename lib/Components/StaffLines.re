open Revery.UI;
open Drawing;




let make = (~width, ~height, ()) => {
  let w_int = int_of_float(width);
  let h_int = int_of_float(height);
  let canvasStyle =
    Style.[
      width(w_int),
      height(h_int),
      position(`Absolute),
      left(0),
      top(0) ];
  <Canvas
      style=canvasStyle
      render={canvasContext => {
        let staffContext = StaffContext.make(~width, ~height, ~canvasContext);
        let draw = x => Drawable.draw(staffContext, x);
        draw(StaffLines.make());
      }}
    />
} 