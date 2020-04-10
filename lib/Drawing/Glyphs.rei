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

module BBox : {
  type coord = {
    x: float,
    y: float
  };

  type t = {
    sw: coord,
    ne: coord
  };

  let get : Name.t => t
}

type t = {
  name: string,
  text: string,
  bbox: BBox.t,
  scale: float
}

let make : Name.t => t;