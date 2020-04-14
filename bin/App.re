open Revery;
open Revery.UI;

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
      <Game
        style=Styles.staff
        height=200.
        width=800.
        params=(Game.Params.Duo(
          {clef:Treble, rangeMin:Solfege.Note.do_(4), rangeMax:Solfege.Note.do_(5)},
          {clef:Bass, rangeMin:Solfege.Note.si(2), rangeMax:Solfege.Note.si(3)}))
      />
    </View>
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
