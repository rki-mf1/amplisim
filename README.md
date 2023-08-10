[![build](https://github.com/krannich479/amplisim/actions/workflows/build.yml/badge.svg)](https://github.com/krannich479/amplisim/actions/workflows/build.yml)
# amplisim
Plain simple amplicon sequence simulator for in-silico genomic sequencing assays

## Installation
### Build from source
```
git clone --recursive https://github.com/Krannich479/amplisim.git
cd amplisim
mkdir build
make -C lib/htslib
make
```
<details><summary> 🍎 macOS system dependencies </summary>
If you are working on an Apple workstation with macOS and want to build amplisim from source you might miss system libraries for openssl and argp. These can be installed using the brew package manager via 
  
```
brew install glib-openssl argp-standalone
```
</details>
