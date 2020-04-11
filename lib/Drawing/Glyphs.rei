module Font : {

  /** Name of the file that contains the font */
  let file : string

  /** Absolute path to the file */
  let absolute : string

  /** Revery Font */
  let font : Revery.Font.t

  /** Extracted Bravura Typeface */
  let typeface : Revery.Draw.Skia.Typeface.t
}


module Name : {
  type t
  let noteheadBlack : t
  let gClef : t
  let fClef : t
  let cClef : t
};

module Text : {
  let get : Name.t => string
};

module Coord : {
  type t = {
    x: float,
    y: float
  };

  let getBBoxSW : Name.t => t;
  let getBBoxNE : Name.t => t;
  let getStemUpSE : Name.t => t;
  let getStemDownNW : Name.t => t;
}

type t = {
  name: Name.t,
  text: string,
}

let make : Name.t => t;

let noteheadBlack : t;
let gClef : t;
let cClef : t;
let fClef : t;