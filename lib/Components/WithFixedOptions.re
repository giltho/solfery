module type OPTIONS = {
  let width: float;
  let height: float;
  let clef: Solfege.Clef.t;
};

module Make = (O: OPTIONS) => {
  open O;

  module StaffLines = {
    let make = StaffLines.make(~height, ~width);
  };

  module Note = {
    let make = Note.make(~height, ~width, ~clef);
  };

  module GameNote = {
    let make = GameNote.make(~height, ~width, ~clef);
  };

  module Staff = {
    let make = Staff.make(~height, ~width, ~clef);
  };

  module Game = {
    let make = Game.make(~height, ~width, ~clef);
  };
};
