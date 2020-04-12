open Drawing;

let make = (~width, ~height, ~note, ~clef, ~x, ~color=Coloring.black, ()) => {
  <Canvas
    width
    height
    render={draw => {
      let note = Note.make(~clef, ~note, ~x, ~color, ());
      draw(note);
    }}
  />;
};
