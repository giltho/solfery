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
 * This should be enough to draw anything.
 */
type t = {
  height: float,
  width: float,
  lineHeight: float,
  nLineSpace: int,
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
  };
};

let dummy = (~height: float, ~width: float) => {
  {
    height,
    width,
    lineHeight: defaultLineHeight,
    nLineSpace: defaultNLineSpace,
    canvasContext: None,
  };
};

let lineHeight = context => context.lineHeight;

let height = context => context.height;

let width = context => context.width;

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
