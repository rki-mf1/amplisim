[![build](https://github.com/krannich479/amplisim/actions/workflows/build.yml/badge.svg)](https://github.com/krannich479/amplisim/actions/workflows/build.yml)
# amplisim
**Plain simple <ins>ampli</ins>con sequence <ins>sim</ins>ulator for in-silico genomic sequencing assays**

1. [Requirements](#requirements)
2. [Installation](#installation)
3. [Operation](#operation)
4. [Input and output](#input-and-output)

## Requirements

TL;DR: no external requirements needed if you are using the pre-build binary.
<details><summary> 🛠️ Details to build from source </summary>
The <i>amplisim</i> software is intended for 64-bit POSIX compliant operating systems and was tested successfully under Ubuntu 22.04 LTS and macOS v12.5.1 (Monterey).
Building <i>amplisim</i> from source requires libraries for <i>lzma</i>, <i>libbz2</i> and <i>libcurl</i> on your system in order to compile <i>htslib</i>.</details>

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
If you are working on an Apple workstation with macOS and want to build <i>amplisim</i> from source you might miss system libraries for <i>openssl</i> and <i>argp</i>. These can be installed using the brew package manager via 
  
```
brew install glib-openssl argp-standalone
```
</details>

### Test your build (optional)
A quick and simple way to test your software binary is to download and run amplisim on some public Sars-Cov-2 data.

```
mkdir testdata && cd testdata
wget https://raw.githubusercontent.com/artic-network/primer-schemes/master/nCoV-2019/V5.3.2/SARS-CoV-2.primer.bed
wget https://www.ebi.ac.uk/ena/browser/api/fasta/MN908947.3
sed 's/>ENA|MN908947|MN908947.3 Severe acute respiratory syndrome coronavirus 2 isolate Wuhan-Hu-1, complete genome./>MN908947.3/g' MN908947.3 > MN908947.3.fasta
cd ..
./amplisim testdata/MN908947.3.fasta testdata/SARS-CoV-2.primer.bed
```

## Operation
### Help page
The most concise way to get familiar with _amplisim_ is to inspect the help page via `amplisim --help`. This will display

```
Usage: amplisim [OPTION...] REFERENCE PRIMERS
amplisim -- a program to simulate amplicon sequences from a reference genome

  -o, --output=FILE          Output to FILE instead of standard output
  -s, --seed=INT             Set a random seed
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version

Mandatory or optional arguments to long options are also mandatory or optional
for any corresponding short options.

Report bugs to https://github.com/rki-mf1/amplisim/issues.
```

### Minimal working examples
The minimal command to run _amplisim_ is to provide a reference genome in FASTA format and a set of primers in BED format (see chapter [Input and output](#input-and-output) for more details).
By default, _amplisim_ prints the amplicons sequences to the standard output, s.t. the user can either store the sequences or forward them to the next program.
```
amplisim <my_reference.fasta> <my_primers.bed> > <my_amplicons.fasta>
```
If you are just interested in the resulting FASTA file you can directly use the `-o` option.
```
amplisim -o <my_amplicons.fasta> <my_reference.fasta> <my_primers.bed>
```

## Input and output
### The primer file (input)
The `PRIMERS` input file is a plain tab-separated textfile with pre-defined columns.
The format of the `PRIMERS` file required by _amplisim_ has to comply with the following properties:
1. The BED format specification. I.e. the first column is a chromosome identifier, and the second and third column are the boundary indexes of a range in the chromosome. The second column is the start index of a primer and the third column is the end index of a primer. The start index should always be strictly smaller than the end index.
2. A pair of primers (forward and reverse primer) is expected to be in consecutive lines in the file.
3. The chromosome identifiers have to be arranged in blocks. I.e. irrespective of the order of the chromosomes, all primers of a particular chromosome have to occur consecutively in the file.

These format properties generally comply with the definitions in [samtools](http://www.htslib.org/doc/samtools-ampliconstats.html#DESCRIPTION) but are slightly more stringent as _amplisim_ currently does not allow alternative primers in a pair.
Directly fitting examples can be found in the artic-network repository for virus primer schemes, e.g. the [primers for Sars-Cov-2](https://github.com/artic-network/primer-schemes/blob/master/nCoV-2019/V5.3.2/SARS-CoV-2.primer.bed).

### The amplicons (output)
The output of _amplisim_ is a stream or plain textfile in the FASTA format.
The header line of each amplicon sequence provides the following information:<br>
```
>amplicon_<amplicon_index>_<replicate_index>
```
where _<amplicon_index>_ is the _i_-th index (i=0...n-1) of the amplicons defined by _n_ primer pairs and _<replicate_index>_ is a unique index across all replicates of all amplicons.
