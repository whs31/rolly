alias t := test
alias td := test-dev
alias tdd := test-dev-no-rebuild
alias doc := mcss-commit
alias v := version

[linux]
default:
  @echo "Usage: just <command> [options]"
  @just --choose

[windows]
default:
  @echo "Usage: just <command> [options]"
  @just --list

[group('conan')]
[doc('Configure release without tests')]
@conan-release:
  conan install . -b missing -o "rolly/*:test=False"

[group('conan')]
[doc('Configure debug without tests')]
@conan-debug:
  conan install . -b missing --settings=build_type=Debug -o "rolly/*:test=False"

[group('conan')]
[doc('Build debug library and tests')]
@conan-debug-test:
  conan install . -b missing --settings=build_type=Debug -o "rolly/*:test=True"

[group('cmake')]
[doc('Configure release without tests')]
@cmake-configure-release:
  cmake --preset conan-release

[group('cmake')]
[doc('Configure debug without tests')]
@cmake-configure-debug:
  cmake --preset conan-debug

[group('cmake')]
[doc('Build release without tests')]
@cmake-build-release:
  cmake --build --preset conan-release

[group('cmake')]
[doc('Build debug without tests')]
@cmake-build-debug:
  cmake --build --preset conan-debug

[group('build')]
[doc('Build release library without tests')]
@build-release:
  just conan-release
  just cmake-configure-release
  just cmake-build-release

[group('build')]
[doc('Build debug library without tests')]
@build-debug:
  just conan-debug
  just cmake-configure-debug
  just cmake-build-debug

[group('build')]
[doc('Build debug library with tests')]
@build-tests:
  just conan-debug-test
  just cmake-configure-debug
  just cmake-build-debug

[group('test')]
[doc('Run tests')]
@test: build-tests
  ./build/Debug/tests/rolly-test

[group('test')]
[doc('Run developer executable')]
@test-dev: build-tests
  echo "Running development executable..."
  ./build/Debug/bin/rolly-dev || echo "(returned non-zero)"

[group('test')]
[doc('Run developer executable without reconfiguration')]
@test-dev-no-rebuild: 
  just cmake-build-debug
  echo "Running development executable..."
  echo " "
  ./build/Debug/bin/rolly-dev || echo "(returned non-zero)"

mcss-folder := "D:/dev/my/m.css/documentation"

[group('doc')]
[linux]
[doc('Remove previously generated documentation')]
@mcss-remove:
  echo "Removing previously generated documentation..."
  rm -rf docs || true

[group('doc')]
[windows]
[doc('Remove previously generated documentation')]
@mcss-remove:
  echo "Removing previously generated documentation..."
  if (Test-Path docs) { Remove-Item -Recurse -Force docs -ErrorAction SilentlyContinue }

[group('doc')]
[doc('Generate m.css documentation')]
@mcss-generate: mcss-remove
  echo "Generating documentation..."
  python3 {{mcss-folder/"doxygen.py"}} './conf.py'

[group('doc')]
[windows]
[doc('Store m.css documentation')]
@mcss-store: mcss-generate
  echo "Storing documentation..."
  Copy-Item -Recurse -Force docs -Destination ..
  if (Test-Path ../docs/xml) { Remove-Item -Recurse -Force ../docs/xml -ErrorAction SilentlyContinue }

[group('doc')]
[linux]
[doc('Store m.css documentation')]
@mcss-store: mcss-generate
  echo "Storing documentation..."
  cp -r docs ..
  rm -rf ../docs/xml || true

[group('doc')]
[windows]
[doc('Restore m.css documentation')]
@mcss-restore:
  echo "Restoring documentation..."
  Copy-item -Force -Recurse -Verbose ../docs/* -Destination docs

[group('doc')]
[linux]
[doc('Restore m.css documentation')]
@mcss-restore:
  echo "Restoring documentation..."
  cp -r ../docs/* docs

[group('doc')]
[linux]
[doc('Remove previously stored documentation')]
@mcss-remove-stored:
  echo "Removing previously stored documentation..."
  rm -rf ../docs || true

[group('doc')]
[windows]
[doc('Remove previously stored documentation')]
@mcss-remove-stored:
  echo "Removing previously stored documentation..."
  if (Test-Path ../docs) { Remove-Item -Recurse -Force ../docs -ErrorAction SilentlyContinue }

[group('doc')]
[doc('Commit generated documentation')]
@mcss-commit: mcss-store
  echo "Committing documentation..."
  git checkout gh-pages
  just mcss-restore
  git add .
  git commit -m "Updated documentation"
  git push origin gh-pages
  just mcss-remove-stored
  git checkout main

[group('misc')]
[doc('Patch version')]
@tag VERSION:
  echo "Patching version..."
  python3 scripts/tag.py -v {{VERSION}}

[group('misc')]
[doc('Show versions')]
@version:
  echo "Current versions is:"
  python3 scripts/tag.py --show

[group('misc')]
[doc('Format code')]
@format:
    find . -iname '*.h' -o -iname '*.cc' | xargs clang-format -i;
    echo "Formatted!"
