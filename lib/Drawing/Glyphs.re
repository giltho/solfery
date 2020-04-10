let ($) = (o, p) => {
  switch (o) {
  | `Assoc(l) => List.assoc(p, l)
  | _ => failwith("Not an object")
  };
};

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

  Console.log(hash);

  let get = Hashtbl.find(hash);
};

module BBox = {
  type coord = {
    x: float,
    y: float,
  };

  type t = {
    sw: coord,
    ne: coord,
  };

  let of_yojson = (j: Yojson.Safe.t) => {
    switch (j) {
    | `Assoc([
        ("bBoxNE", `List([`Float(xne), `Float(yne)])),
        ("bBoxSW", `List([`Float(xsw), `Float(ysw)])),
      ]) => {
        sw: {
          x: xne,
          y: yne,
        },
        ne: {
          x: xsw,
          y: ysw,
        },
      }
    | _ => failwith("Wrong Bbox: " ++ Yojson.Safe.pretty_to_string(j))
    };
  };

  let hash: Hashtbl.t(Name.t, t) = {
    let hash = Hashtbl.create(3000);
    let data =
      Yojson.Safe.from_file(Revery.Environment.getAssetPath(metadataFile));
    let data =
      switch (data $ "glyphBBoxes") {
      | `Assoc(l) => l
      | _ => failwith("Invalid metadata file, impossible to get glyphBBoxes")
      };

    let addDataToHash = ((name, o)) =>
      Hashtbl.add(hash, name, of_yojson(o));

    List.iter(addDataToHash, data);
    hash;
  };

  let get = Hashtbl.find(hash);
};

type t = {
  name: string,
  text: string,
  bbox: BBox.t,
  scale: float,
};

let make = name => {
  let text = Text.get(name);
  let bbox = BBox.get(name);
  ();
};

/* let noteheadBlack = textFromGlyphNames([Name.noteheadBlack]); */
