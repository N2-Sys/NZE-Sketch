# Toward Nearly-Zero-Error Sketching via Compressive Sensing

#### Description of all files and directories

|------data/:  the processed CAIDA trace

|------include/: the common header files

|&emsp;&emsp;|----common.h：definitions of flowkeys

|&emsp;&emsp;|----BloomFilter.h：operations of Bloom Filter

|&emsp;&emsp;|----AwareHash.h：the hash function

|&emsp;&emsp;|----Eigen/：to solve the problem of compressive sensing

|------SeqSketch/: source code of SeqSketch

|&emsp;&emsp;|----HashTable.h：operations of the hash table

|&emsp;&emsp;|----CMsketch.h：operations of the fractional sketch

|&emsp;&emsp;|----sketch.h：operations of SeqSketch

|&emsp;&emsp;|----parameters.h：all parameters

|------EmbedSketch/: source code of EmbedSketch



#### Compile the program

You can compile the program with the shell script:

```shell
$ ./build.sh
```

You can also run the following constructions:

```shell
$ mkdir build
$ mkdir bin
$ cd build
$ cmake .. -DEXECUTABLE_OUTPUT_PATH=../bin && make
```

Then, the executable file is in the `bin` directory.

**Note: After changing the parameters in `parameters.h`, you need to re-execute the `make` command.**



#### Run the program

You can run the program with the following constructions:

```shell
$ cd bin
$ ./seq [Optional: path_of_the_data]
```

 or:

```
./bin/seq path_of_the_data
```

The default dataset is in the `data` directory.

You can also generate your own data with the [pcap parser](https://github.com/Dustin-He/PcapParser).



