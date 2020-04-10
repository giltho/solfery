type t('a) = {
  content: 'a,
  draw: StaffContext.t => unit,
};

let draw = (cs, x) => x.draw(cs);
