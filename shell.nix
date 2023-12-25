{ pkgs ? import <nixpkgs> {
  config.allowUnfree = true;
} }:

let
  clang = pkgs.llvmPackages_14.clang-unwrapped;

  # NOTE: so... emscripten in Nix basically doesn't work as of writing.
  # First, it tries to write cache to an unwritable /nix/store location.
  # That's fixed further down in our EM_CONFIG variable.
  # Second, the node_modules/.bin stubs are lost during the install phase.
  # That's fixed by either this commented code (didn't quite work but don't
  # know why not) or the next override.
  # emscripten = pkgs.stdenv.mkDerivation {
  #   name = "emscripten-that-works";
  #   version = pkgs.emscripten.version;

  #   buildInputs = [ pkgs.emscripten pkgs.nodejs-18_x ];

  #   src = pkgs.emscripten.outPath;

  #   dontBuild = true;

  #   llvmEnv = pkgs.emscripten.llvmEnv;
  #   meta = pkgs.emscripten.meta;

  #   installPhase = ''
  #     mkdir $out
  #     cp -r * $out

  #     cd $out/share/emscripten/node_modules
  #     mkdir .bin
  #     cd .bin
  #     ln -s ../acorn/bin/acorn                   acorn
  #     ln -s ../browserslist/cli.js               browserslist
  #     ln -s ../update-browserslist-db/cli.js     browserslist-lint
  #     ln -s ../envinfo/dist/cli.js               envinfo
  #     ln -s ../es-check/index.js                 es-check
  #     ln -s ../eslint/bin/eslint.js              eslint
  #     ln -s ../eslint-config-prettier/bin/cli.js eslint-config-prettier
  #     ln -s ../google-closure-compiler/cli.js    google-closure-compiler
  #     ln -s ../html-minifier-terser/cli.js       html-minifier-terser
  #     ln -s ../import-local/fixtures/cli.js      import-local-fixture
  #     ln -s ../js-yaml/bin/js-yaml.js            js-yaml
  #     ln -s ../which/bin/node-which              node-which
  #     ln -s ../prettier/bin/prettier.cjs         prettier
  #     ln -s ../resolve/bin/resolve               resolve
  #     ln -s ../rimraf/bin.js                     rimraf
  #     ln -s ../terser/bin/terser                 terser
  #     ln -s ../webpack/bin/webpack.js            webpack
  #     ln -s ../webpack-cli/bin/cli.js            webpack-cli
  #   '';
  # };
  emscripten = pkgs.emscripten.overrideAttrs (previous: {
    postInstall = ''
      cd $out/share/emscripten/node_modules
      mkdir .bin
      cd .bin
      # Sad, but we have to ln -s manually because npm install tries to access
      # the internet which seems forbidden during install phase (understandably).
      ln -s ../acorn/bin/acorn                   acorn
      ln -s ../browserslist/cli.js               browserslist
      ln -s ../update-browserslist-db/cli.js     browserslist-lint
      ln -s ../envinfo/dist/cli.js               envinfo
      ln -s ../es-check/index.js                 es-check
      ln -s ../eslint/bin/eslint.js              eslint
      ln -s ../eslint-config-prettier/bin/cli.js eslint-config-prettier
      ln -s ../google-closure-compiler/cli.js    google-closure-compiler
      ln -s ../html-minifier-terser/cli.js       html-minifier-terser
      ln -s ../import-local/fixtures/cli.js      import-local-fixture
      ln -s ../js-yaml/bin/js-yaml.js            js-yaml
      ln -s ../which/bin/node-which              node-which
      ln -s ../prettier/bin/prettier.cjs         prettier
      ln -s ../resolve/bin/resolve               resolve
      ln -s ../rimraf/bin.js                     rimraf
      ln -s ../terser/bin/terser                 terser
      ln -s ../webpack/bin/webpack.js            webpack
      ln -s ../webpack-cli/bin/cli.js            webpack-cli
    '';
  });

in pkgs.mkShell {
  nativeBuildInputs = with pkgs; [
    zig
    zls
    glfw
    clang
    emscripten
    glibc
    python3
    xorg.libX11
    xorg.libXcursor
    xorg.libXrandr
    xorg.libXinerama
    xorg.libXi
  ];

  EM_CONFIG = pkgs.writeText ".emscripten" ''
    EMSCRIPTEN_ROOT = '${emscripten}/share/emscripten'
    LLVM_ROOT = '${emscripten.llvmEnv}/bin'
    BINARYEN_ROOT = '${pkgs.binaryen}'
    NODE_JS = '${pkgs.nodejs-18_x}/bin/node'
    CACHE = '${toString ./.cache}'
  '';

  shellHook = ''
    clangbase="${pkgs.lib.makeLibraryPath [ emscripten.llvmEnv ]}/clang"
    clanginclude="$clangbase/$(ls $clangbase | head -n1)/include"

    cat << YML > .clangd
CompileFlags:
  Add:
    - '-I$PWD/src'
    - '-I$clanginclude'
    - '-I${pkgs.glibc.dev}/include'
    - '-I$(realpath ../raylib/src)'
    - '-I$PWD/.cache/sysroot/include'
YML
  '';
}
