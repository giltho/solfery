let ($?) = (o, p) => {
  switch (o) {
  | `Assoc(l) => List.assoc_opt(p, l)
  | _ => None
  };
};

let ($) = (o, p) => Option.get(o $? p);

let metadataFile = "bravura_metadata.json";

module Font = {
  let file = "Bravura.otf";
  let absolute = Revery.Environment.getAssetPath(file);
  let font =
    switch (Revery.Font.load(file)) {
    | Ok(f) => f
    | Error(m) => failwith(m)
    };
  let typeface = Revery.Font.getSkiaTypeface(font);
};

module Name = {
  type t = string;
  let noteheadBlack = "noteheadBlack";
  let cClef = "cClef";
  let fClef = "fClef";
  let gClef = "gClef";
};

module Text = {
  let glyphNamesFile = "glyphnames.json";

  let fromCodePoint = codepoint => {
    let codepoint = String.sub(codepoint, 2, String.length(codepoint) - 2);
    let codepoint = int_of_string("0x" ++ codepoint);
    let b = Buffer.create(5);
    Buffer.add_utf_8_uchar(b, Uchar.of_int(codepoint));
    Buffer.contents(b);
  };

  /* Populate the hashtable */
  let hash: Hashtbl.t(Name.t, string) = {
    let hash = Hashtbl.create(3000);
    let data =
      Yojson.Safe.from_file(Revery.Environment.getAssetPath(glyphNamesFile));
    let data =
      switch (data) {
      | `Assoc(l) => l
      | _ => failwith("Wrong glyphnames.json file")
      };

    let addDataToHash = ((name, o)) => {
      let codepoint =
        switch (o $ "codepoint") {
        | `String(c) => c
        | codepoint =>
          failwith("Invalid Codepoint: " ++ Yojson.Safe.to_string(codepoint))
        };
      let text = fromCodePoint(codepoint);
      Hashtbl.add(hash, name, text);
    };

    List.iter(addDataToHash, data);
    hash;
  };

  let get = Hashtbl.find(hash);
};

module Coord = {
  type t = {
    x: float,
    y: float,
  };

  let ofYojson = j => {
    switch (j) {
    | `List([`Float(x), `Float(y)]) => {x, y}
    | _ => failwith("Wrong coordinates")
    };
  };

  let (hashBBoxSW, hashBBoxNE, hashStemUpSE, hashStemDownNW) = {
    let hashBBoxSW = Hashtbl.create(100);
    let hashBBoxNE = Hashtbl.create(100);

    let hashStemUpSE = Hashtbl.create(100);
    let hashStemDownNW = Hashtbl.create(100);

    let data =
      Yojson.Safe.from_file(Revery.Environment.getAssetPath(metadataFile));
    let dataBBox =
      switch (data $ "glyphBBoxes") {
      | `Assoc(l) => l
      | _ => failwith("Invalid metadata file, impossible to get glyphBBoxes")
      };

    let f = ((name, o)) => {
      let bBoxNE = ofYojson(o $ "bBoxNE");
      let bBoxSW = ofYojson(o $ "bBoxSW");
      Hashtbl.add(hashBBoxNE, name, bBoxNE);
      Hashtbl.add(hashBBoxSW, name, bBoxSW);
    };

    List.iter(f, dataBBox);

    let dataAnchor =
      switch (data $ "glyphsWithAnchors") {
      | `Assoc(l) => l
      | _ =>
        failwith("Invalid metadata file, impossible to get glyphsAnchors")
      };

    let f = ((name, o)) => {
      o
      $? "stemUpSE"
      |> Option.map(ofYojson)
      |> Option.iter(Hashtbl.add(hashStemUpSE, name));

      o
      $? "stemDownNW"
      |> Option.map(ofYojson)
      |> Option.iter(Hashtbl.add(hashStemDownNW, name));
    };

    List.iter(f, dataAnchor);

    (hashBBoxSW, hashBBoxNE, hashStemUpSE, hashStemDownNW);
  };

  let getBBoxSW = Hashtbl.find(hashBBoxSW);
  let getBBoxNE = Hashtbl.find(hashBBoxNE);

  let getStemUpSE = Hashtbl.find(hashStemUpSE);
  let getStemDownNW = Hashtbl.find(hashStemDownNW);
};

type t = {
  name: string,
  text: string,
};

let make = name => {
  let text = Text.get(name);
  {name, text};
};

let noteheadBlack = make(Name.noteheadBlack);

let cClef = make(Name.cClef);
let fClef = make(Name.fClef);
let gClef = make(Name.gClef);
