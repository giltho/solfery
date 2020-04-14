open Revery;
open Revery.UI;

module Params = {
  type single = {
    clef: Solfege.Clef.t,
    rangeMin: Solfege.Note.t,
    rangeMax: Solfege.Note.t,
  };

  type t =
    | Single(single)
    | Duo(single, single);
};

module Row = {
  let make = (~children, ~width as w, ~height as h, ()) => {
    let style =
      Style.[
        flexDirection(`Row),
        alignItems(`Stretch),
        justifyContent(`Center),
        flexGrow(1),
        width(int_of_float(w)),
        height(int_of_float(h)),
      ];
    <View style> ...children </View>;
  };
};

module State = {
  type guess =
    | Correct
    | Incorrect
    | Waiting;

  type whichStaff =
    | Up
    | Down;

  type t = {
    notes: list((float, Solfege.Note.t, whichStaff, guess)),
    target: int /* Target */
  };

  let make = (~notes, ~target) => {
    {notes, target};
  };

  let rec split = (~curr=0, ~currUp=0, ~currDown=0, state) => {
    switch (state.notes) {
    | [] => ({target: (-1), notes: []}, {target: (-1), notes: []})
    | [(x, n, Up, g), ...rest] =>
      let (restUp, restDown) =
        split(
          ~curr=curr + 1,
          ~currUp=currUp + 1,
          ~currDown,
          {...state, notes: rest},
        );
      let targetUp =
        if (state.target === curr) {
          currUp;
        } else {
          restUp.target;
        };
      (
        {target: targetUp, notes: [(x, n, Up, g), ...restUp.notes]},
        restDown,
      );
    | [(x, n, Down, g), ...rest] =>
      let (restUp, restDown) =
        split(
          ~curr=curr + 1,
          ~currDown=currDown + 1,
          ~currUp,
          {...state, notes: rest},
        );
      let targetDown =
        if (state.target === curr) {
          currDown;
        } else {
          restDown.target;
        };
      (
        restUp,
        {target: targetDown, notes: [(x, n, Down, g), ...restDown.notes]},
      );
    };
  };

  let getTarget = state => {
    List.nth(state.notes, state.target);
  };

  let translateLeft = (state, xMove) => {
    {
      ...state,
      notes:
        List.map(((x, n, s, g)) => (x -. xMove, n, s, g), state.notes),
    };
  };

  let rec setTarget = (~curr=0, targ, state) =>
    if (curr === state.target) {
      {...state, notes: [targ, ...List.tl(state.notes)]};
    } else {
      let restState =
        setTarget(
          ~curr=curr + 1,
          targ,
          {...state, notes: List.tl(state.notes)},
        );
      {...state, notes: [List.hd(state.notes), ...restState.notes]};
    };

  let guess = (~guessed: Solfege.Note.pure, state) => {
    let (x, n, s, _) = getTarget(state);
    let newState =
      if (n.note === guessed) {
        setTarget((x, n, s, Correct), state);
      } else {
        setTarget((x, n, s, Incorrect), state);
      };

    {...newState, target: newState.target + 1};
  };

  type action =
    | TimeTick(Time.t)
    | Guess(Solfege.Note.pure);
};

module Button = {
  let make = (~note: Solfege.Note.pure, ~height as h: float, ~dispatch, ()) => {
    let clickableStyle =
      Style.[
        position(`Relative),
        backgroundColor(Colors.lightGrey),
        justifyContent(`Center),
        alignItems(`Center),
        flexGrow(1),
        height(int_of_float(h)),
        margin(10),
      ];
    let viewStyle =
      Style.[
        position(`Relative),
        justifyContent(`Center),
        alignItems(`Center),
      ];
    let textStyle =
      Style.[
        color(Colors.black),
        fontFamily("Roboto-Regular.ttf"),
        fontSize(32.),
        textWrap(TextWrapping.NoWrap),
      ];

    <Revery.UI.Components.Clickable
      style=clickableStyle onClick={() => dispatch(State.Guess(note))}>
      <View style=viewStyle>
        <Text style=textStyle text={Solfege.Note.pureToString(note)} />
      </View>
    </Revery.UI.Components.Clickable>;
  };
};

module KeyboardInput = {
  type state = {
    ref: option(node),
    hasFocus: bool,
  };

  type action =
    | Focused(bool)
    | SetRef(node);

  let reducer = (action, state) =>
    switch (action) {
    | Focused(v) => {...state, hasFocus: v}
    | SetRef(v) => {...state, ref: Some(v)}
    };

  let%component make = (~dispatch as parentDispatch, ()) => {
    let%hook (v, dispatch) =
      Hooks.reducer(~initialState={ref: None, hasFocus: false}, reducer);

    let%hook () =
      Hooks.effect(
        Always,
        () => {
          if (!v.hasFocus) {
            switch (v.ref) {
            | Some(v) => Focus.focus(v)
            | None => ()
            };
          };
          None;
        },
      );

    let onBlur = () => {
      dispatch(Focused(false));
    };

    let onFocus = () => {
      dispatch(Focused(true));
    };

    let letterD = 100;
    let letterR = 114;
    let letterM = 109;
    let letterF = 102;
    let letterS = 115;
    let letterL = 108;
    let letterI = 105;

    let respondToKeys = (e: NodeEvents.keyEventParams) => {
      switch (e.keycode) {
      | v when v === letterD => parentDispatch(State.Guess(Do))
      | v when v === letterR => parentDispatch(State.Guess(Re))
      | v when v === letterM => parentDispatch(State.Guess(Mi))
      | v when v === letterF => parentDispatch(State.Guess(Fa))
      | v when v === letterS => parentDispatch(State.Guess(Sol))
      | v when v === letterL => parentDispatch(State.Guess(La))
      | v when v === letterI => parentDispatch(State.Guess(Si))
      | _ => ()
      };
    };

    <View
      ref={r => dispatch(SetRef(r))}
      onBlur
      onFocus
      style=Style.[position(`Absolute), width(1), height(1)]
      onKeyDown=respondToKeys
    />;
  };
};

let computeOpacity = (~xEndClef: float, ~xLimit: float, x) =>
  max(0., min(1., (x -. xEndClef) /. (xLimit -. xEndClef)));

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

let makeInitialState =
    (~width, ~height, ~spacing, ~randomNote, ~alternate, ()) => {
  open Drawing;
  let randomNote = i => {
    let staff =
      if (alternate && i mod 2 == 1) {
        State.Down;
      } else {
        Up;
      };
    let note = randomNote(staff);
    let x = Float.of_int(i) *. spacing +. width;
    let guess = State.Waiting;
    (x, note, staff, guess);
  };
  let usableWidth =
    width -. StaffContext.xClef(StaffContext.dummy(~width, ~height));
  let numberNotes = Int.of_float(Float.ceil(usableWidth /. spacing));
  let notes = List.init(numberNotes, randomNote);
  State.make(~notes, ~target=0);
};

let refresh =
    (~xClef, ~xLimit, ~spacing, ~randomNote, ~elapsed, ~alternate, state) => {
  let (xTarget, _, _, _) = State.getTarget(state);
  let sElapsed = Time.toFloatSeconds(elapsed);
  let vFactor = 1. /. 1000.;

  let xMoveFor25FPS = max(0., (vFactor *. (xTarget -. xLimit)) ** 2.);
  let xMove = xMoveFor25FPS *. sElapsed *. 25.;
  let rec shiftAndProduce =
          (~lastX=0., ~lastStaff=State.Up, ~toCreate=0, state: State.t)
          : State.t => {
    switch (state.notes) {
    | [] when toCreate <= 0 => {...state, notes: []}
    | [] =>
      let newX = lastX +. spacing;
      let newStaff =
        if (alternate && lastStaff === Up) {
          State.Down;
        } else {
          Up;
        };
      let newState =
        shiftAndProduce(~lastX=newX, ~toCreate=toCreate - 1, ~lastStaff=newStaff, state);
      {
        ...state,
        notes: [
          (newX, randomNote(newStaff), newStaff, Waiting),
          ...newState.notes,
        ],
      };
    | [(x, n, s, g), ...rest] =>
      if (x < xClef) {
        shiftAndProduce(
          ~lastX=x,
          ~toCreate=toCreate + 1,
          ~lastStaff=s,
          {target: state.target - 1, notes: rest},
        );
      } else {
        let restState =
          shiftAndProduce(~lastX=x, ~toCreate, ~lastStaff=s, {...state, notes: rest});
        {...restState, notes: [(x, n, s, g), ...restState.notes]};
      }
    };
  };
  shiftAndProduce(State.translateLeft(state, xMove));
};

let reducer =
    (~xClef, ~xLimit, ~spacing, ~randomNote, ~alternate, action, state) => {
  switch (action) {
  | State.TimeTick(elapsed) =>
    refresh(
      ~xClef,
      ~xLimit,
      ~spacing,
      ~randomNote,
      ~elapsed,
      ~alternate,
      state,
    )
  | Guess(guessed) => State.guess(~guessed, state)
  };
};

let gameNotesOfState =
    (~width, ~height, ~clef, ~xEndClef, ~xLimit, state: State.t) => {
  List.mapi(
    (i, (x, note, _, guess)) => {
      let color =
        if (guess === State.Incorrect) {
          Drawing.Coloring.red;
        } else {
          Drawing.Coloring.black;
        };
      let opacity = computeOpacity(~xEndClef, ~xLimit, x);
      <GameNote
        width
        height
        clef
        note
        x
        focus={i === state.target}
        color
        opacity
      />;
    },
    state.notes,
  );
};

let%component make =
              (
                ~width as totalWidth: float,
                ~height as totalHeight: float,
                ~params: Params.t,
                ~style=Style.emptyViewStyle,
                (),
              ) => {
  let (xClef, xEndClef) = {
    open Drawing;
    let sc = StaffContext.dummy(~width=totalWidth, ~height=totalWidth);
    let xClef = StaffContext.xClef(sc);
    let w = glyphWidth(totalWidth, totalHeight, Glyphs.gClef);
    (xClef, xClef +. w);
  };

  let alternate =
    switch (params) {
    | Single(_) => false
    | Duo(_) => true
    };

  let (minUp, maxUp, minDown, maxDown) = {
    switch (params) {
    | Single(p) => (
        p.rangeMin,
        p.rangeMax,
        Solfege.Note.do_(4),
        Solfege.Note.do_(4),
      )
    | Duo(p1, p2) => (p1.rangeMin, p1.rangeMax, p2.rangeMin, p2.rangeMax)
    };
  };

  let xLimit = xLimit(totalWidth, totalHeight);
  let spacing = spacing(totalWidth, totalHeight);
  let randomNote = s => {
    switch (s) {
    | State.Up => Solfege.Note.randomBetween(minUp, maxUp)
    | State.Down => Solfege.Note.randomBetween(minDown, maxDown)
    };
  };
  let reducer = reducer(~xClef, ~xLimit, ~spacing, ~randomNote, ~alternate);

  let initialState =
    makeInitialState(
      ~width=totalWidth,
      ~height=totalHeight,
      ~spacing,
      ~randomNote,
      ~alternate,
      (),
    );

  let%hook (state, dispatch) = Hooks.reducer(~initialState, reducer);

  let _dispose =
    Revery.Tick.interval(t => dispatch(State.TimeTick(t)), Time.zero);

  let staves =
    switch (params) {
    | Single(p) =>
      <Staff width=totalWidth height=totalHeight clef={p.clef}>
        {React.listToElement(
           gameNotesOfState(
             ~width=totalWidth,
             ~height=totalHeight,
             ~clef=p.clef,
             ~xEndClef,
             ~xLimit,
             state,
           ),
         )}
      </Staff>
    | Duo(p1, p2) =>
      let (stateUp, stateDown) = State.split(state);
      React.listToElement(
        <>
          <Staff width=totalWidth height=totalHeight clef={p1.clef}>
            {React.listToElement(
               gameNotesOfState(
                 ~width=totalWidth,
                 ~height=totalHeight,
                 ~clef=p1.clef,
                 ~xEndClef,
                 ~xLimit,
                 stateUp,
               ),
             )}
          </Staff>
          <Staff width=totalWidth height=totalHeight clef={p2.clef}>
            {React.listToElement(
               gameNotesOfState(
                 ~width=totalWidth,
                 ~height=totalHeight,
                 ~clef=p2.clef,
                 ~xEndClef,
                 ~xLimit,
                 stateDown,
               ),
             )}
          </Staff>
        </>,
      );
    };

  <View style>
    <KeyboardInput dispatch />
    { staves }
    <Row width=totalWidth height=totalHeight>
      {React.listToElement(
         List.map(
           note => <Button dispatch note height={totalHeight /. 2.} />,
           [Solfege.Note.Do, Re, Mi, Fa, Sol, La, Si],
         ),
       )}
    </Row>
  </View>;
};
