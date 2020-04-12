let draw = (sc: StaffContext.t) => {
  let nthLineRect = n => {
    let overshoot = (StaffContext.lineHeight(sc) -. 1.) /. 2.;
    let yCenter = StaffContext.nthVisibleLineY(sc, n);
    let yStart = yCenter -. overshoot;
    let yEnd = yCenter +. overshoot;
    let xStart = 0.;
    let xEnd = StaffContext.width(sc);
    Skia.Rect.makeLtrb(xStart, yStart, xEnd, yEnd);
  };

  let paint = Skia.Paint.make();
  Skia.Paint.setColor(paint, Coloring.grey);

  for (n in 0 to 4) {
    let rect = nthLineRect(n);
    StaffContext.Draw.drawRect(~rect, ~paint, sc);
  };
};

let make = () => {
  {Drawable.content: (), draw};
};
