open Revery;
open Revery.UI;
/* open Revery.UI.Components; */

let make =
    (
      ~style=Style.emptyViewStyle,
      ~width: float,
      ~height: float,
      ~clef: Solfege.Clef.t,
      /* ~note: Note.t, */
      ~children=React.empty,
      (),
    ) => {
  let w_int = int_of_float(width);
  let h_int = int_of_float(height);

  let canvasStyle =
    Style.[width(w_int), height(h_int), backgroundColor(Colors.white)];

  <View style>
    <View style=canvasStyle>
      <StaffLines width height />
      <Clef width height clef x=10. />
      children
    </View>
  </View>;
};
