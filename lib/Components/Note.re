open Drawing;

let make = (~width, ~height, ~note, ~clef, ~x, ()) => {
  <Canvas
    width
    height
    render={draw => {
      let note = Note.make(~clef, ~note, ~x, ());
      draw(note);
    }}
  />;
};
