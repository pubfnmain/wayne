{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
    in {
      packages.${system}.default = pkgs.stdenv.mkDerivation {
        name = "wayne";
        
        nativeBuildInputs = with pkgs; [
          clang
          pkg-config
          gnumake
          wayland-scanner
        ];

        buildInputs = with pkgs; [
          wlroots_0_19
          wayland
          pixman
          libxkbcommon
          wayland-protocols
        ];

        shellHook = ''
          echo "Alive Inside"
        '';
      };
    };
}
