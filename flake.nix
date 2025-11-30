{
  description = "wayne: a minimal stacking Wayland compositor";

  inputs.nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
    in {
      devShells.${system}.default = pkgs.mkShell {
        nativeBuildInputs = with pkgs; [
          clang-tools
          wayland-scanner
          bear
          meson
          samurai
          pkg-config
          gdb
        ];

        buildInputs = with pkgs; [
          libinput
          wlroots
          wayland
          wayland-protocols
          pixman
          libxkbcommon
        ];
      };
    };
}
