open Revery;
open Revery.UI;
open Revery.UI.Components;
open Revery.Draw;

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

    let inner = [flexDirection(`Row), alignItems(`FlexEnd)];
  };

  <View style=Styles.container>
    <StaffDisplay width=500. height=200. clef=Treble note=Solfege.Note.fa(4) />
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
