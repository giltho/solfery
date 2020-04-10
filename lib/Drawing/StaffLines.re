let draw = (sc: StaffContext.t) => {
  let nthLineRect = n => {
    let overshoot = (sc.lineHeight -. 1.) /. 2.;
    let actualN = StaffContext.lineOfVisibleLine(n);
    let yCenter = StaffContext.nthLineY(sc, actualN);
    let yStart = yCenter -. overshoot;
    let yEnd = yCenter +. overshoot;
    let xStart = 0.;
    let xEnd = sc.width;
    Skia.Rect.makeLtrb(xStart, yStart, xEnd, yEnd);
  };

  let paint = Skia.Paint.make();
  Skia.Paint.setColor(paint, Coloring.black);

  for (n in 0 to 4) {
    let rect = nthLineRect(n);
    StaffContext.Draw.drawRect(~rect, ~paint, sc);
  };
};
