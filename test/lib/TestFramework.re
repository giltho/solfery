let getEnvOpt = s =>
  try(Some(Sys.getenv(s))) {
  | Not_found => None
  };

let getRoot = () => {
  switch (getEnvOpt("MUSIC_PROJECT_ROOT")) {
  | Some(dir) => dir
  | None =>
    failwith(
      "Expected `MUSIC_PROJECT_ROOT` environment variable to be set "
      ++ "before running tests.",
    )
  };
};

include Rely.Make({
  let config =
    Rely.TestFrameworkConfig.initialize({
      snapshotDir: "test/lib/__snapshots__",
      projectDir: getRoot(),
    });
});
