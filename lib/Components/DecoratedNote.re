open Drawing;

let make =
    (
      ~width,
      ~height,
      ~clef,
      ~note,
      ~x,
      ~opacity=1.,
      ~color=Coloring.black,
      ~focus=false,
      (),
    ) => {
  <Canvas
    width
    height
    render={draw => {
      draw(GameNote.make(~opacity, ~note, ~color, ~focus, ~clef, ~x, ()))
    }}
  />;
};
