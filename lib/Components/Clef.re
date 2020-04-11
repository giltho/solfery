open Drawing;

let make = (~width, ~height, ~clef, ~x, ()) => {
  <Canvas width height render={draw => {draw(Clef.make(~clef, ~x))}} />;
};
