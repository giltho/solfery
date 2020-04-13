open Revery;
open Revery.UI;


module Row = {
  let make = (~children, ~width as w, ~height as h, ()) => {
    let style =
      Style.[
        flexDirection(`Row),
        alignItems(`Stretch),
        justifyContent(`Center),
        flexGrow(1),
        width(int_of_float(w)),
        height(int_of_float(h))
      ];
    <View style> ...children </View>;
  };
};

module State = {

  type guess =
    | Correct
    | Incorrect
    | Waiting

  type t = {
    notes: list((float, Solfege.Note.t, guess)), /* Circular queue */
    target: int, /* Target */
  };

  let make = (~notes, ~target) => {
    {notes, target};
  };

  let getTarget = state => {
    List.nth(state.notes, state.target);
  };

  let translateLeft = (state, xMove) => {
    {...state, notes: List.map(((x, n, g)) => (x -. xMove, n, g), state.notes)};
  };

  let rec setTarget = (~curr=0, targ, state) => {
    if (curr === state.target) {
      {...state, notes: [targ, ...List.tl(state.notes)] }
    } else {
      let restState = setTarget(~curr = curr +  1, targ, { ...state, notes: List.tl(state.notes) });
      {...state, notes: [List.hd(state.notes), ...restState.notes] }
    }
  };

  let guess = (~guessed: Solfege.Note.pure, state) => {
    let (x, n, _) = getTarget(state);
    let newState =
      if (n.note === guessed) {
        setTarget((x, n, Correct), state)
      } else {
        setTarget((x, n, Incorrect), state)
      };

    {...newState, target: newState.target + 1};
  };

  type action =
    | TimeTick(Time.t)
    | Guess(Solfege.Note.pure);
};


module Button = {
  let make =
    (
      ~note: Solfege.Note.pure,
      ~height as h: float,
      ~dispatch,
      ()
    ) => {
      let clickableStyle =
      Style.[
        position(`Relative),
        backgroundColor(Colors.lightGrey),
        justifyContent(`Center),
        alignItems(`Center),
        flexGrow(1),
        height(int_of_float(h)),
        /* Min width */
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

    <Revery.UI.Components.Clickable style=clickableStyle onClick={() => dispatch(State.Guess(note))} >
      <View style=viewStyle> <Text style=textStyle text=Solfege.Note.pureToString(note) /> </View>
    </Revery.UI.Components.Clickable>;
    }
}

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

    let respondToKeys = (_: NodeEvents.keyEventParams) => {
      parentDispatch(State.Guess(Solfege.Note.Do));
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
  let randomNote = i => {
    let note = randomNote();
    let x = Float.of_int(i) *. spacing +. width;
    let guess = State.Waiting;
    (x, note, guess);
  };
  let usableWidth =
    width -. StaffContext.xClef(StaffContext.dummy(~width, ~height));
  let numberNotes = Int.of_float(Float.ceil(usableWidth /. spacing));
  let notes = List.init(numberNotes, randomNote);
  State.make(~notes, ~target=0);
};

let refresh = (~xClef, ~xLimit, ~spacing, ~randomNote, ~elapsed, state) => {
  let (xTarget, _, _) = State.getTarget(state);
  let sElapsed = Time.toFloatSeconds(elapsed);
  let vFactor = 1. /. 1000.;

  let xMoveFor25FPS = max(0., (vFactor *. (xTarget -. xLimit)) ** 2.);
  let xMove = xMoveFor25FPS *. sElapsed *. 25.;
  let rec shiftAndProduce = (~lastX=0., ~toCreate=0, state: State.t) : State.t=> {
    switch (state.notes) {
    | [] when toCreate <= 0 => { ...state, notes:[] }
    | [] =>
      let newX = lastX +. spacing;
      let newState = shiftAndProduce(~lastX=newX, ~toCreate=toCreate - 1, state);
      {
        ...state,
        notes:[
        (newX, randomNote(), Waiting),
        ...newState.notes,
      ]
    };
    | [(x, n, g), ...rest] =>
      if (x < xClef) {
        shiftAndProduce(~lastX=x, ~toCreate=toCreate + 1, {target:state.target-1, notes:rest});
      } else {
        let restState = shiftAndProduce(~lastX=x, ~toCreate=toCreate, {...state, notes:rest});
        {
          ...restState,
          notes:[(x, n, g), ...restState.notes]
        };
      }
    };
  };
  shiftAndProduce(State.translateLeft(state, xMove));
};

let reducer = (~xClef, ~xLimit, ~spacing, ~randomNote, action, state) => {
  switch (action) {
  | State.TimeTick(elapsed) =>
    refresh(~xClef, ~xLimit, ~spacing, ~randomNote, ~elapsed, state)
  | Guess(guessed) => State.guess(~guessed, state)
  };
};

let gameNotesOfState =
    (~width, ~height, ~clef, ~xClef, ~xLimit, state: State.t) => {
  List.mapi(
    (i, (x, note, guess)) => {
      let color = if (guess === State.Incorrect) { Drawing.Coloring.red } else { Drawing.Coloring.black };
      <GameNote
        width
        height
        clef
        note
        x
        focus={i === state.target}
        color
        opacity={computeOpacity(~xClef, ~xLimit, x)}
      />
    },
    state.notes,
  );
};

let%component make =
              (
                ~width as totalWidth: float,
                ~height as totalHeight: float,
                ~clef: Solfege.Clef.t,
                ~style=Style.emptyViewStyle,
                (),
              ) => {
  let xClef = {
    Drawing.(
      StaffContext.xClef(
        StaffContext.dummy(~width=totalWidth, ~height=totalWidth),
      )
    );
  };

  let min = Solfege.Note.do_(4);
  let max = Solfege.Note.do_(5);

  let xLimit = xLimit(totalWidth, totalHeight);
  let spacing = spacing(totalWidth, totalHeight);
  let randomNote = () => Solfege.Note.randomBetween(min, max);
  let reducer = reducer(~xClef, ~xLimit, ~spacing, ~randomNote);

  let initialState =
    makeInitialState(
      ~width=totalWidth,
      ~height=totalHeight,
      ~spacing,
      ~randomNote,
      (),
    );

  let%hook (state, dispatch) = Hooks.reducer(~initialState, reducer);

  let _dispose =
    Revery.Tick.interval(t => dispatch(State.TimeTick(t)), Time.zero);
  <View style>
    <KeyboardInput dispatch />
    <Staff width=totalWidth height=totalHeight clef>
      {React.listToElement(
         gameNotesOfState(
           ~width=totalWidth,
           ~height=totalHeight,
           ~clef,
           ~xClef,
           ~xLimit,
           state,
         ),
       )}
    </Staff>
    <Row width=totalWidth height=totalHeight>
        {React.listToElement(
          List.map((note) => <Button dispatch note height=(totalHeight /. 2.)/>, [Solfege.Note.Do, Re, Mi, Fa, Sol, La, Si])
        )}
    </Row>
  </View>;
};
