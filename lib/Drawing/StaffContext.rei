type t;


let make : (~height: float, ~width: float, ~canvasContext: Revery.Draw.CanvasContext.t) => t;

let dummy : (~height: float, ~width: float) => t;

let isDummy : t => bool;

let lineSpacing : t => float;

let lineHeight : t => float

let height : t => float

let width : t => float;

let xClef : t => float;

let nthLineY : (t,  int) => float;

let lineOfVisibleLineUnsafe : int => int;

let lineOfVisibleLine : int => int;

let nthVisibleLineY : (t, int) => float;

let glyphBoxRect : (~glyph: Glyphs.t, ~x: float, ~y: float, t) => (float, float, float, float);

module Draw : {
  open Revery.Draw.Skia;

  let drawRect : (~paint:Paint.t, ~rect: Rect.t, t) => unit;

  let drawText : (~x: float, ~y: float, ~paint: Paint.t, ~text: string, t) => unit;

  let drawPath: (~path: Path.t, ~paint: Paint.t, t) => unit;
}