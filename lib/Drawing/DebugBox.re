open Revery.Draw;

type t = {
  x: float,
  y: float,
  rad: float,
  color: Skia.Color.t,
};

let draw = (db, sc) => {
  let xStart = db.x -. db.rad;
  let xEnd = db.x +. db.rad;
  let yStart = db.y -. db.rad;
  let yEnd = db.y +. db.rad;
  let rect = Skia.Rect.makeLtrb(xStart, yStart, xEnd, yEnd);
  let paint = Skia.Paint.make();
  Skia.Paint.setColor(paint, db.color);
  StaffContext.Draw.drawRect(~rect, ~paint, sc);
};

let make = (~x, ~y, ~rad, ~color=Coloring.blue, ()) => {
  let content = {x, y, rad, color};
  Drawable.make(content, draw);
};

let makeAndDraw = (~x, ~y, ~rad, ~color=Coloring.blue, sc) => {
  let db = make(~x, ~y, ~rad, ~color, ());
  Drawable.draw(sc, db);
};
