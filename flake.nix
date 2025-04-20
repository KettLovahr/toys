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
            clangStdenv
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
        devShells.${system}.default = pkgs.mkShell {
            buildInputs = deps;

            LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath deps;
        };
    };
}
