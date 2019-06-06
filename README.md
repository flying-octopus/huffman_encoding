# Huffman coding
This is my project for second semester of computer science course.

## Problem
Write a program, which creates a Huffman's code for given text file (program needs to count instances of each character for the text), and then encodes the file. Write a program that decodes an encoded file.

### Algorithm
The process begins with the leaf nodes containing the probabilities of the symbol they represent. Then, the process takes the two nodes with smallest probability, and creates a new internal node having these two nodes as children. The weight of the new node is set to the sum of the weight of the children. We then apply the process again, on the new internal node and on the remaining nodes (i.e., we exclude the two leaf nodes), we repeat this process until only one node remains, which is the root of the Huffman tree.

The simplest construction algorithm uses a priority queue where the node with lowest probability is given highest priority:

1. Create a leaf node for each symbol and add it to the priority queue.
1. While there is more than one node in the queue:
	1. Remove the two nodes of highest priority (lowest probability) from the queue
	1. Create a new internal node with these two nodes as children and with probability equal to the sum of the two nodes' probabilities.
	1. Add the new node to the queue.
1. The remaining node is the root node and the tree is complete.

### Instalation
`make`

### Manual
Options
`-c` : Program will compress file given as a second argument. Compressed file's name is going to be a third argument, example:
`./huffman -c test.txt test` will compress file `test.txt`, a compressed file will be called `test`.

'-d' : Program will decompress file given as a second argument. Decompressed file's name is going to be a third argument, example:
`./huffman -d test test.txt` will decompress file `test`, a decompressed fille will be called `test.txt`.
