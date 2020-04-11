open Revery;
open Revery.UI;
module Components =
  Solfery.Components.WithFixedOptions.Make({
    let width = 800.;
    let height = 200.;
    let clef = Solfege.Clef.Treble;
  });

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
  Components.(
    <View style=Styles.container>
      <Staff style=Styles.staff>
        <Note note={Solfege.Note.sol(4)} x=200. />
        <Note note={Solfege.Note.si(4)} x=200. />
      </Staff>
    </View>
  );
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
