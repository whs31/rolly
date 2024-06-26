## Helper scripts
Collection of utility scripts for the _floppy_ project

### `tag.py`
Script to update all versions in respective files in project and optionally push new git tag.

##### Usage
The following command will:
- Update version in **CMakeLists.txt** to `1.2.3`
- Update version in **conanfile.py** to `1.2.3-alpha-patch1`, if **conanfile.py** exists
- Update version in **Doxyfile** to `1.2.3-alpha-patch1`, if **Doxyfile** exists
- Create new git tag `1.2.3-alpha-patch1`
- Push git tag `1.2.3-alpha-patch1` to default remote

```shell
python3 tag.py -v 1.2.3-alpha-patch1 --git
```

##### Parameters
- `-g`, `--git` Push new git tag to default remote. Default: `False`
- `-v`, `--version` New version. Can be in format `X.Y.Z` or `X.Y.Z[.*]`

> [!WARNING]
> `git` must be installed and available in the system to push new git tag.
