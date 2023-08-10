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

### Test your build
A quick and simple way to test your software binary is to download and run amplisim on some public Sars-Cov-2 data.

```
mkdir testdata && cd testdata
wget https://raw.githubusercontent.com/artic-network/primer-schemes/master/nCoV-2019/V5.3.2/SARS-CoV-2.primer.bed
wget https://www.ebi.ac.uk/ena/browser/api/fasta/MN908947.3
sed 's/>ENA|MN908947|MN908947.3 Severe acute respiratory syndrome coronavirus 2 isolate Wuhan-Hu-1, complete genome./>MN908947.3/g' MN908947.3 > MN908947.3.fasta
cd ..
./amplisim testdata/MN908947.3.fasta testdata/SARS-CoV-2.primer.bed
```

