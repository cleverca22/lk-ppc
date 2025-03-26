{ stdenv
, enableDebugging
, dtc
, gdb
, nodejs
, path
, pkgsCross
, qemu
}:

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
    dtc
    nodejs
    gdb
    pkgsCross.ppc-embedded.stdenvNoLibs.cc
    pkgsCross.ppc64.stdenv.cc
    (enableDebugging (qemu.override { hostCpuTargets = [ "ppc64-softmmu" "ppc-softmmu" ]; }))
  ];
  shellHook = ''
    unset CC OBJDUMP LD OBJCOPY
  '';
  hardeningDisable = [ "all" ];
}
