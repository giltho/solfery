open Revery.UI;

module State = {
  type t = {
    notes: list((float, Solfege.Note.t)), /* Circular queue */
    target: int /* Target */
  };

  let make = (~notes, ~target) => {
    {notes, target};
  };

  let getTarget = state => {
    List.nth(state.notes, state.target);
  };

  let translateLeft = (state, xMove) => {
    {...state, notes: List.map(((x, n)) => (x -. xMove, n), state.notes)};
  };

  type action =
    | TimeTick;
};

let computeOpacity = (~xClef: float, ~xLimit: float, x) =>
  max(0., min(1., (x -. xClef) /. (xLimit -. xClef)));

let glyphWidth = (width, height, glyph) => {
  open Drawing;
  let (xa, _, xb, _) =
    StaffContext.glyphBoxRect(
      ~glyph,
      ~x=0.,
      ~y=0.,
      StaffContext.dummy(~height, ~width),
    );
  xb -. xa;
};

let widthNote = (width, height) => {
  glyphWidth(width, height, Drawing.Glyphs.noteheadBlack);
};

let widthClef = (width, height) => {
  /* Using the treble clef as a reference for everything, to ensure alignment */
  glyphWidth(
    width,
    height,
    Drawing.Glyphs.gClef,
  );
};

let xLimit = (width, height) => {
  open Drawing;
  let xClef = StaffContext.xClef(StaffContext.dummy(~width, ~height));
  let wClef = widthClef(width, height);
  xClef +. 3. *. wClef;
};

let spacing = (width, height) => 3. *. widthNote(width, height);

let makeInitialState = (~width, ~height, ~spacing, ~randomNote, ()) => {
  open Drawing;
  let randomNote = {
    let note = randomNote();
    i => {
      let x = Float.of_int(i) *. spacing +. width;
      (x, note);
    };
  };
  let usableWidth =
    width -. StaffContext.xClef(StaffContext.dummy(~width, ~height));
  let numberNotes = Int.of_float(Float.ceil(usableWidth /. spacing));
  let notes = List.init(numberNotes, randomNote);
  State.make(~notes, ~target=0);
};

let reduceOnTick = (~xClef, ~xLimit, ~spacing, ~randomNote, state) => {
  let (xTarget, _) = State.getTarget(state);
  let vFactor = 1. /. 20.;
  let xMove = max(0., vFactor *. (xTarget -. xLimit));
  let rec shiftAndProduce = (~lastX=0., ~toCreate=0, notes) => {
    switch (notes) {
    | [] when toCreate <= 0 => []
    | [] =>
      let newX = lastX +. spacing;
      [
        (newX, randomNote()),
        ...shiftAndProduce(~lastX=newX, ~toCreate=toCreate - 1, []),
      ];
    | [(x, n), ...rest] =>
      if (x < xClef) {
        shiftAndProduce(~lastX=x, ~toCreate=toCreate + 1, rest);
      } else {
        [(x, n), ...shiftAndProduce(~lastX=x, ~toCreate, rest)];
      }
    };
  };
  let stateMoved = State.translateLeft(state, xMove);
  {...stateMoved, notes: shiftAndProduce(stateMoved.notes)};
};

let reducer = (~xClef, ~xLimit, ~spacing, ~randomNote, action, state) => {
  switch (action) {
  | State.TimeTick =>
    reduceOnTick(~xClef, ~xLimit, ~spacing, ~randomNote, state)
  };
};

let gameNotesOfState =
    (~width, ~height, ~clef, ~xClef, ~xLimit, state: State.t) => {
  List.mapi(
    (i, (x, note)) => {
      <GameNote
        width
        height
        clef
        note
        x
        focus={i === state.target}
        color=Drawing.Coloring.black
        opacity={computeOpacity(~xClef, ~xLimit, x)}
      />
    },
    state.notes,
  );
};

let%component make =
              (
                ~width: float,
                ~height: float,
                ~clef: Solfege.Clef.t,
                ~style=Style.emptyViewStyle,
                (),
              ) => {
  let xClef = {
    Drawing.(StaffContext.xClef(StaffContext.dummy(~width, ~height)));
  };

  let xLimit = xLimit(width, height);
  let spacing = spacing(width, height);
  let randomNote = () => fst(Solfege.Clef.noteOnLine(clef));
  let reducer = reducer(~xClef, ~xLimit, ~spacing, ~randomNote);

  let initialState =
    makeInitialState(~width, ~height, ~spacing, ~randomNote, ());

  let%hook (state, dispatch) = Hooks.reducer(~initialState, reducer);

  let _dispose =
    Revery.Tick.interval(
      _ => dispatch(State.TimeTick),
      Revery.Time.ofFloatSeconds(0.001),
    );

  <Staff width height clef style>
    {React.listToElement(
       gameNotesOfState(~width, ~height, ~clef, ~xClef, ~xLimit, state),
     )}
  </Staff>;
};
