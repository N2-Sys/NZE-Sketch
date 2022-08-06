# Toward Nearly-Zero-Error Sketching via Compressive Sensing

#### Description of all files and directories

|------data/:  the processed CAIDA trace

|------include/: the common header files

|	  |----common.h：definitions of flowkeys

|	  |----BloomFilter.h：operations of Bloom Filter

|	  |----AwareHash.h：the hash function

|	  |----Eigen/：to solve the problem of compressive sensing

|------SeqSketch/: source code of SeqSketch

|	  |----HashTable.h：operations of the hash table

|	  |----CMsketch.h：operations of the fractional sketch

|	  |----sketch.h：operations of SeqSketch

|	  |----parameters.h：all parameters

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



