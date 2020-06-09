#! /bin/bash

# 测试压缩与解压缩过程是否正确的临时脚本

make clean
make

./lar -c -f a.lar BinTools.o Coder.o Huffman.o main.o
sha1sum BinTools.o
rm *.o
./lar -x -f a.lar
sha1sum BinTools.o

./lar -c -f a.lar BinTools.o Coder.o Huffman.o main.o
sha1sum Coder.o
rm *.o
./lar -x -f a.lar
sha1sum Coder.o

./lar -c -f a.lar BinTools.o Coder.o Huffman.o main.o
sha1sum Huffman.o
rm *.o
./lar -x -f a.lar
sha1sum Huffman.o

./lar -c -f a.lar BinTools.o Coder.o Huffman.o main.o
sha1sum main.o
rm *.o
./lar -x -f a.lar
sha1sum main.o
