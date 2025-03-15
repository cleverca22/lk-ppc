{ stdenv, pkgsCross, path }:

let
  ppc64 = import path {
    crossSystem = {
      config = "powerpc64-unknown-none-eabi";
    };
    system = "x86_64-linux";
  };
in
stdenv.mkDerivation {
  name = "shell";
  nativeBuildInputs = [
    #ppc64.stdenv.cc
    pkgsCross.ppc64.stdenv.cc
    pkgsCross.ppc-embedded.stdenvNoLibs.cc
  ];
  shellHook = ''
    unset CC OBJDUMP LD OBJCOPY
  '';
  hardeningDisable = [ "all" ];
}
