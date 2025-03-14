{ stdenv, pkgsCross }:

stdenv.mkDerivation {
  name = "shell";
  nativeBuildInputs = [ pkgsCross.ppc64.stdenv.cc ];
  shellHook = ''
    unset CC OBJDUMP LD OBJCOPY
  '';
  hardeningDisable = [ "all" ];
}
