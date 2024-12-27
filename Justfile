alias v   := version
alias fmp := fix_msvc_preset

profile := "default"
compat := "True"

[linux]
default:
  @echo "Usage: just <command> [options]"
  @just --choose

[windows]
default:
  @echo "Usage: just <command> [options]"
  @just --list

[group('misc')]
[doc('Patch version')]
@tag VERSION:
  echo "Patching version..."
  python scripts/tag.py -v {{VERSION}}

[group('misc')]
[doc('Show versions')]
@version:
  echo "Current versions is:"
  python scripts/tag.py --show

[group('misc')]
[doc('Format code')]
@format:
    find . -iname '*.h' -o -iname '*.cc' | xargs clang-format -i;
    echo "Formatted!"

[group('misc')]
[doc('Deploy package locally')]
@deploy:
  rm -rf build
  conan create . -b missing --user=whs31 --channel=dev -pr={{profile}}

[group('build')]
[doc('Clean build directory')]
@clean:
  echo "Cleaning..."
  @rm -rf build

[group('build')]
[doc('Configure')]
@configure BUILD_TYPE:
  echo "Configuring in {{BUILD_TYPE}} mode"
  conan install . --build=missing --settings=build_type={{BUILD_TYPE}} -pr={{profile}} -o "rolly/*:test=True" -o "rolly/*:compat={{compat}}"

[group('misc')]
[doc('Fix MSVC preset')]
@fix_msvc_preset:
    sed -i 's/v143/v144/g' build/generators/CMakePresets.json