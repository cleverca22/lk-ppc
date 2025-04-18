{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-23.05";
    utils.url = "github:numtide/flake-utils";
    xenon-utils.url = "github:cleverca22/xenon-utils";
  };
  outputs = { self, nixpkgs, utils, xenon-utils }:
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
          config = "powerpc64-unknown-none-gnuabielfv2";
        };
        system = "x86_64-linux";
      };
      x86_64-linux = nixpkgs.legacyPackages.${system};
    };
    pkgs = pkgsLut.${system};
  in {
    devShell = pkgs.callPackage ./devshell.nix { inherit (xenon-utils.packages.${system}) elf-converter; };
  }));
}
