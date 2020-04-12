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
  <Revery.UI.Opacity opacity>
    <Canvas
      width
      height
      render={draw => {
        draw(GameNote.make(~note, ~color, ~focus, ~clef, ~x, ()))
      }}
    />
  </Revery.UI.Opacity>;
};
