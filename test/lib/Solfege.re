open TestFramework;
open Solfery;

describe("Solfege", ({test, _}) => {
  test("toInt(do(0)) should be 0", ({expect, _}) => {
    Solfege.Note.(expect.int(toInt(do_(0))).toBe(0))
  });

  test("toInt(do(1)) should be -7", ({expect, _}) => {
    Solfege.Note.(expect.int(toInt(do_(1))).toBe(-7))
  });

  test("toInt(mi(3)) should be -30", ({expect, _}) => {
    Solfege.Note.(expect.int(toInt(mi(3))).toBe(-23))
  });

  test("compare(do(0), do(7)) should be greater than 0", ({expect, _}) => {
    Solfege.Note.(expect.bool(compare(do_(0), do_(7)) > 0).toBe(true))
  });

  test("compare(Re, Mi) should be 1", ({expect, _}) => {
    open Solfege.Note;
    let r = compare(re(2), mi(2));
    expect.int(r).toBe(1);
  });

  test("ofInt should be the inverse of toInt", ({expect, _}) => {
    open Solfege.Note;
    for (i in -70 to 0) {
      expect.int(toInt(ofInt(i))).toBe(i);
    }
  })
});
