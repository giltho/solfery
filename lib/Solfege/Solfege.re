module Note = {
  type pure =
    | /** C */
      Do
    | /** D */
      Re
    | /** E */
      Mi
    | /** F */
      Fa
    | /** G */
      Sol
    | /** A */
      La
    | /** B */
      Si;

  let pureToInt = n => {
    switch (n) {
    | Do => 0
    | Re => 1
    | Mi => 2
    | Fa => 3
    | Sol => 4
    | La => 5
    | Si => 6
    };
  };

  let pureOfInt = n => {
    switch (n) {
    | 0 => Do
    | 1 => Re
    | 2 => Mi
    | 3 => Fa
    | 4 => Sol
    | 5 => La
    | 6 => Si
    | _ => raise(Invalid_argument("Invalid int for pure note"));
    }
  }

  type t = {
    note: pure,
    octave: int,
  };

  let make = (~note, ~octave) => {
    {note, octave};
  };

  let do_ = octave => make(~note=Do, ~octave);

  let re = octave => make(~note=Re, ~octave);

  let mi = octave => make(~note=Mi, ~octave);

  let fa = octave => make(~note=Fa, ~octave);

  let sol = octave => make(~note=Sol, ~octave);

  let la = octave => make(~note=La, ~octave);

  let si = octave => make(~note=Si, ~octave);
  /**
   * `toInt(do_(0))` is `0`, `toInt(re(0))` is `-1` etc..
   */
  let toInt = n => - (n.octave * 7 + pureToInt(n.note));

  let ofInt = n => {
    let n = -n;
    let octave = n / 7;
    let note = pureOfInt(n mod 7);
    { note, octave }
  };

  /**
   * Gives the distance between two notes.
   * the result of noteDist(a, b) is (a - b).
   * Note that the higher the note, the lower its index.
   * This is due to the fact that in the graphical engine,
   * lower y is higher.
   */
  let compare = (a: t, b: t) => {
    toInt(a) - toInt(b);
  };

  let randomBetween = {
    Random.self_init();
    (noteA, noteB) => {
    let iA = toInt(noteA);
    let iB = toInt(noteB);
    let range = iA - iB;
    let r = Random.int(range + 1);
    ofInt(iB + r)
  }
}
};

module Clef = {
  type t =
    | Treble
    | Bass
    | Alto;

  /**
   * Which note is on which visible line depending on key.
   * For example, on the Trebble key, the Sol 4 (G 4) is on the 4th line from the top (index 3).
   */
  let noteOnLine = clef => {
    switch ((clef: t)) {
    | Treble => (Note.make(~note=Sol, ~octave=4), 3)
    | Bass => (Note.make(~note=Fa, ~octave=3), 1)
    | Alto => (Note.make(~note=Do, ~octave=4), 2)
    };
  };
};
