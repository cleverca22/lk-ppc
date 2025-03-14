{
  inputs = {
    utils.url = "github:numtide/flake-utils";
  };
  outputs = { self, nixpkgs, utils }:
  (utils.lib.eachSystem [ "x86_64-linux" "ppc64" "ppc32" ] (system:
  let
    pkgsLut = {
      ppc32 = import nixpkgs {
        crossSystem = {
          config = "powerpc-none-eabi";
          libc = "newlib";
        };
        system = "x86_64-linux";
      };
      ppc64 = import nixpkgs {
        crossSystem = {
          config = "powerpc64-unknown-linux-gnuabielfv2";
        };
        system = "x86_64-linux";
      };
      x86_64-linux = nixpkgs.legacyPackages.${system};
    };
    pkgs = pkgsLut.${system};
  in {
    devShell = pkgs.callPackage ./devshell.nix {};
  }));
}
