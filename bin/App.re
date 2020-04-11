open Revery;
open Revery.UI;
open Solfery.Components;

let main = () => {
  module Styles = {
    open Style;

    let container = [
      position(`Absolute),
      justifyContent(`Center),
      alignItems(`Center),
      bottom(0),
      top(0),
      left(0),
      right(0),
    ];

    let staff = [paddingVertical(10)];
  };

  <View style=Styles.container>
    <Staff style=Styles.staff width=800. height=200. clef=Treble >
      <Note clef=Treble note=Solfege.Note.sol(4) x=200. width=800. height=200./>
      <Note clef=Treble note=Solfege.Note.si(4) x=200. width=800. height=200./>
    </Staff>
  </View>;
};

let init = app => {
  Revery.App.initConsole();

  Timber.App.enable();
  Timber.App.setLevel(Timber.Level.perf);

  let win = App.createWindow(app, "Some music app!");

  let _: Revery.UI.renderFunction = UI.start(win, <main />);
  ();
};

App.start(init);
