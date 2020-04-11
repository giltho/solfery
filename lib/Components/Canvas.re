open Revery.UI;
open Drawing;

let make = (~width, ~height, ~render, ~children=React.empty, ()) => {
  let w_int = int_of_float(width);
  let h_int = int_of_float(height);

  let canvasStyle =
    Style.[
      position(`Absolute),
      left(0),
      top(0),
      width(w_int),
      height(h_int),
    ];
  <Canvas
    style=canvasStyle
    render={canvasContext => {
      let staffContext = StaffContext.make(~width, ~height, ~canvasContext);
      let draw = x => Drawable.draw(staffContext, x);
      render(draw);
    }}>
    ...children
  </Canvas>;
};
