module type S = {
  type t;

  let draw : t => Revery.Draw.CanvasContext.t => unit
};