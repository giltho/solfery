/**
 * # StaffContext
 *
 * Internal engine of how a staff works.
 * A Staff can display 21 different notes.
 * (We go up to 3 lines above and 3 lines under the standard 5 lines)
 *
 * Accordingly, the view is split into 12 line spaces (spaces between two lines, including invisible ones)
 */

let defaultNLineSpace = 12;

/**
 * Number of pixels a visible line takes vertically.
 */
let defaultLineHeight = 3.;

/**
 * By default, the clef is drawn 10px from the left.
 */
let defaultXClef = 10.;

/**
 * This should be enough to draw anything.
 */
type t = {
  height: float,
  width: float,
  lineHeight: float,
  nLineSpace: int,
  xClef: float,
  canvasContext: option(Revery.Draw.CanvasContext.t),
};

let make =
    (
      ~height: float,
      ~width: float,
      ~canvasContext: Revery.Draw.CanvasContext.t,
    ) => {
  {
    height,
    width,
    canvasContext: Some(canvasContext),
    lineHeight: defaultLineHeight,
    nLineSpace: defaultNLineSpace,
    xClef: defaultXClef,
  };
};

let dummy = (~height: float, ~width: float) => {
  {
    height,
    width,
    lineHeight: defaultLineHeight,
    nLineSpace: defaultNLineSpace,
    xClef: defaultXClef,
    canvasContext: None,
  };
};

let lineHeight = context => context.lineHeight;

let height = context => context.height;

let width = context => context.width;

let xClef = context => context.xClef;

/**
 * Actual number of pixels between the center of two visible lines.
 */
let lineSpacing = context =>
  context.height /. float_of_int(context.nLineSpace);

let nthLineY = (context: t, n: int) =>
  float_of_int(n) *. lineSpacing(context);

let lineOfVisibleLine = (n: int) => {
  if (n < 0 || n > 4) {
    raise(
      Invalid_argument(
        "There are only 5 visible lines, cannot look for the"
        ++ string_of_int(n + 1)
        ++ "th.",
      ),
    );
  };
  /** This should be computed from nLineSpace */ n + 4;
};

let nthVisibleLineY = (context: t, n: int) =>
  nthLineY(context, lineOfVisibleLine(n));

let isDummy = sc => Option.is_none(sc.canvasContext);

let glyphBoxRect = (~glyph: Glyphs.t, ~x: float, ~y: float, context: t) => {
  let name = glyph.name;
  let ls = lineSpacing(context);
  let Glyphs.Coord.{x: lBox, y: bBox} = Glyphs.Coord.getBBoxSW(name);
  let Glyphs.Coord.{x: rBox, y: tBox} = Glyphs.Coord.getBBoxNE(name);
  let xMin = x +. lBox *. ls;
  let xMax = x +. rBox *. ls;
  let yMin = y -. tBox *. ls;
  let yMax = y -. bBox *. ls;
  (xMin, yMin, xMax, yMax);
};

module Draw = {
  open Revery.Draw;

  let drawRect = (~paint, ~rect, staffContext) =>
    Option.iter(
      sc => CanvasContext.drawRect(~paint, ~rect, sc),
      staffContext.canvasContext,
    );

  let drawText = (~x, ~y, ~paint, ~text, staffContext) =>
    Option.iter(
      sc => CanvasContext.drawText(~x, ~y, ~paint, ~text, sc),
      staffContext.canvasContext,
    );

  let drawPath = (~path, ~paint, staffContext) =>
    Option.iter(
      sc => CanvasContext.drawPath(~path, ~paint, sc),
      staffContext.canvasContext,
    );
};
