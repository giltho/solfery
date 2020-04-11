open Drawing;

let make = (~width, ~height, ()) =>
  <Canvas width height render={draw => {draw(StaffLines.make())}} />;
