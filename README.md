Split paired FASTQ files into smaller files with a target number of reads each.

## Requirements

* a C++ compiler with C++11 support

## Download and installation

Either clone the repository:

```
$ git clone https://github.com/fnadalin/fastqsplit.git
```

or download and decompress the tarball archive:

```
$ wget https://github.com/fnadalin/fastqsplit/-/archive/master/fastqsplit-master.tar.gz
$ tar -xzf fastqsplit-master.tar.gz fastqsplit
```

To compile the code:

```
$ cd fastqsplit
$ make
```

The installation takes < 1 second on a desktop computer.
The software has been tested on version v1.0.0.

## Instructions

To run fastqsplit:

```
$ ./fastqsplit <R1.fastq> <R2.fastq> <nseq> <outdir>
```

where nseq is the target number of reads per file.
