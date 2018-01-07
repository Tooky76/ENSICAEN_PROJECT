
# TPE ENSICAEN

This project deals with the segmentation of an image which help to switch from a raw image to homogeneous contiguous regions.

## Installing

To install the project you need to follow this step by step series of instructions to run the project from the source code of the project.


As the project could be compiled with a makefile, here are the instructions to write for running the project.

```
make
```

To launch the project just write

```
./bin/main <image_filename> <nb_blocks_col> <nb_blocks_row> <threshold>
```


Simple and easy.

## Examples

```
./bin/main ./../images/images/fille.ppm 10 10 0.5
```
```
./bin/main ./../images/images/house.ppm 30 30 1.0
````
```
./bin/main ./../images/images/fleurs.ppm 50 50 1.7
```

