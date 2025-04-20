{
    description = "A very basic flake";

    inputs = {
        nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    };

    outputs = { self, nixpkgs }: let
        system = "x86_64-linux";
        pkgs = nixpkgs.legacyPackages.${system};
        deps = with pkgs; [
            pkg-config
            libclang.lib
            clang-tools
            clang
            raylib

            libGL
            glfw
            xorg.libX11
            xorg.libX11.dev
            xorg.libXcursor
            xorg.libXinerama
            xorg.libXrandr
            xorg.libXi
        ];
    in {
        devShells.${system}.default = pkgs.mkShell.override {stdenv = pkgs.clangStdenv;} {
            nativeBuildInputs = deps;
            buildInputs = deps;

            LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath deps;
            LIBCLANG_PATH = "${pkgs.libclang.lib}/lib";
        };
    };
}
