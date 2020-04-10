let fontFile = "Bravura.otf";

let metadataFile = "bravura_metadata.json"

let font =
  switch (Revery.Font.load(fontFile)) {
  | Ok(f) => f
  | Error(m) => failwith(m)
  };

let typeface = Revery.Font.getSkiaTypeface(font);

let glyphNameFile = "glyphnames.json";

let ($) = (o, p) =>
  switch (o) {
  | `Assoc(l) =>
    switch (List.find_opt(((s, _)) => String.equal(s, p), l)) {
    | Some((_, x)) => Some(x)
    | None => None
    }
  | _ => None
  };

let ($?) = (o, p) =>
  switch (o) {
  | Some(o) => o $ p
  | None => None
  };

let glyphNames = 

let glyphNames = Yojson.Safe.from_file(Revery.Environment.getAssetPath(glyphNameFile));

let textFromGlyphNames = names => {
  open Yojson.Safe;

  let ucharOfName = (name) => {
    let codepoint =
      switch (glyphNames $ name $? "codepoint") {
      | Some(`String(x)) => x
      | _ => failwith("Could not retrieve glyph with name " ++ name)
      };
    
    /* Codepoint has U+xxx form, we need a number */
    let codepoint = String.sub(codepoint, 2, String.length(codepoint) - 2);
    let codepoint = int_of_string("0x" ++ codepoint);
    Uchar.of_int(codepoint)
  };
  let b = Buffer.create(100);
  List.iter(Buffer.add_utf_8_uchar(b), List.map(ucharOfName, names));
  Buffer.contents(b);
};


module GlyphNames = {
  let noteheadBlack = "noteheadBlack"
};

module BBox = {

  type coord = {
    x: float,
    y: float
  }

  type t = {
    sw: coord,
    ne: coord
  }

  let of_yojson = (j : Yojson.Safe.t) => {
    switch j {
    | `Assoc [
      ("bBoxNE", `List [`Float xne, `Float yne]),
      ("bBoxSW", `List [`Float xsw, `Float ysw])
    ] => { sw: { x: xne, y: yne}, ne: { x: xsw, y: ysw} }
    | _ => failwith("Wrong Bbox: " ++ Yojson.Safe.pretty_to_string(j))
    }
  }

  let fromName
};

type t = {
  name: string,
  text: string,
  bbox: BBox.t,
  scale: float
}

let make = (name: string) => {
  let text = textFromGlyphNames([name]);
  let bbox = 
}

let noteheadBlack = textFromGlyphNames([GlyphNames.noteheadBlack]);