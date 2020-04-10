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

module Box : {
  type coord = {
    x: float,
    y: float
  };

  type t = {
    sw: coord,
    ne: coord
  };

  let getBBox : Name.t => t
}

type t = {
  name: string,
  text: string,
  bbox: Box.t,
}

let make : Name.t => t;

let noteheadBlack : t;
let gClef : t;
let cClef : t;
let fClef : t;