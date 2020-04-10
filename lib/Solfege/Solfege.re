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
  let toInt = n => - n.octave * 7 - pureToInt(n.note);

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
