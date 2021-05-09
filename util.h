//
// STARTER CODE: util.h
//
// TODO:Mosrour Tafadar; Spring 2021, CS 251
//

#pragma once
#include <string>
#include <fstream>
#include "priorityqueue.h"
#include "hashmap.h"

typedef hashmap hashmapF;
typedef unordered_map <int, string> hashmapE;
struct HuffmanNode {
    int character;
    int count;
    HuffmanNode* zero;
    HuffmanNode* one;
};
//
// *This method frees the memory allocated for the Huffman tree.
//
void freeTree(HuffmanNode* node) {
    if (node == nullptr) {
        return;
    }
    else{
        freeTree(node -> zero);
        freeTree(node -> one);
        delete node;
    }
}
//
// *This function build the frequency map.  If isFile is true, then it reads
// from filename.  If isFile is false, then it reads from a string filename.
//
void frequencyHelper(string filename, hashmapF &map) {
    int mapV = 1;
     for (auto value : filename) {
          mapV = 1;
          if (map.containsKey(value)) {
                mapV = map.get(value) + 1;
                map.put(value, mapV);
            } else {
                map.put(value, mapV);
            }
      }
}
void buildFrequencyMap(string filename, bool isFile, hashmapF &map) {
    const int PSEUDO_EOF = 256;
    int mapV = 1;
    char value;
    if (isFile == true) {
    ifstream infile(filename);
    while (!infile.eof()) {
        infile.get(value);
        mapV = 1;
        if (!infile.fail()) {
            if (map.containsKey(value)) {
                mapV = map.get(value) + 1;
                map.put(value, mapV);
            } else {
                map.put(value, mapV);
            }
        }
    }
    } else {
        frequencyHelper(filename, map);
       }
    map.put(PSEUDO_EOF, mapV);
}
HuffmanNode* buildEncodingTree(hashmapF &map) {
    const int NOT_A_CHAR = 257;
    priorityqueue <HuffmanNode*> pq;
    for ( auto keys:map.keys() ) {
        HuffmanNode* node = new HuffmanNode;
        node -> character = keys;
        node -> count = map.get(keys);
        node->zero = nullptr;
        node -> one = nullptr;
        pq.enqueue(node, node->count);
    }
    while (pq.Size() > 1) {
        HuffmanNode* key1 = pq.dequeue();
        HuffmanNode* key2 = pq.dequeue();
        HuffmanNode* Nnode = new HuffmanNode;
        Nnode -> character =  NOT_A_CHAR;
        Nnode -> count = (key1-> count) + (key2-> count);
        Nnode -> zero = key1;
        Nnode -> one = key2;
        pq.enqueue(Nnode, Nnode -> count);
    }
    return pq.peek();
}
//
// *Recursive helper function for building the encoding map.
//
void _buildEncodingMap(HuffmanNode* node, hashmapE &encodingMap, string str) {
    if (node == nullptr) {
        return;
    }
    if (node -> zero == nullptr && node -> one == nullptr) {
        encodingMap.insert({node -> character, str});
    } else {
        _buildEncodingMap(node -> zero, encodingMap, str + '0');
        _buildEncodingMap(node -> one, encodingMap, str + '1');
    }
}
//
// *This function builds the encoding map from an encoding tree.
//
hashmapE buildEncodingMap(HuffmanNode* tree) {
    hashmapE encodingMap;
    string emptyStr = "";
     _buildEncodingMap(tree, encodingMap, emptyStr);
    return encodingMap;  // TO DO: update this return
}

//
// *This function encodes the data in the input stream into the output stream
// using the encodingMap.  This function calculates the number of bits
// written to the output stream and sets result to the size parameter, which is
// passed by reference.  This function also returns a string representation of
// the output file, which is particularly useful for testing.
//
string encode(ifstream& input, hashmapE &encodingMap, ofbitstream& output, int &size, bool makeFile) {
    string Tstring = "";  //  to store the Binary representation
    char key;  //  to read from file;
    string ExractingValue = "";  //  to  extrac value from map
    string ExractingEof;  // for extrac  EOF
    while (!input.eof()) {
        input.get(key);
        if (!input.fail()) {
        ExractingValue = encodingMap.at(key);
        Tstring += ExractingValue;  //  summing up total values
        }
    }
    ExractingEof = encodingMap.at(256);  // extracting EOF
    Tstring += ExractingEof;  //  summing it with total string
    if (makeFile == true) {
        for ( auto &each : Tstring ) {
            output.writeBit(each-48);
            size = size +  1;
        }
    }
    return Tstring;  // TO DO: update this return
}
//
// *This function decodes the input stream and writes the result to the output
// stream using the encodingTree.  This function also returns a string
// representation of the output file, which is particularly useful for testing.
//
//
int decodeHelper(ifbitstream &input, HuffmanNode* encodingTree) {
    if (encodingTree-> character != NOT_A_CHAR)
        return  encodingTree-> character;
    int bit;
    bit = input.readBit();
    if (bit == 0) {
        return decodeHelper(input, encodingTree-> zero);
    } else {
       return  decodeHelper(input, encodingTree->one);
    }
}
string decode(ifbitstream &input, HuffmanNode* encodingTree, ofstream &output) {
    const int PSEUDO_EOF = 256;
    int key;
    bool flag = true;
    string s = "";
    while (flag) {
       key = decodeHelper(input, encodingTree);
        if (key == PSEUDO_EOF) break;
        s += (char)key;
    }
    return s;
}
//
// *This function completes the entire compression process.  Given a file,
// filename, this function (1) builds a frequency map; (2) builds an encoding
// tree; (3) builds an encoding map; (4) encodes the file (don't forget to
// include the frequency map in the header of the output file).  This function
// should create a compressed file named (filename + ".huf") and should also
// return a string version of the bit pattern.
//
string compress(string filename) {
    ifstream infile(filename);
    int size = 0;
    hashmapE compressF;
    ofbitstream output(filename + ".huf");
    hashmapF map;
    buildFrequencyMap(filename, true, map);
    HuffmanNode* tree = buildEncodingTree(map);
    compressF =   buildEncodingMap(tree);
    output << map;
    string codeStr = encode(infile, compressF, output, size, true);
    output.close();
    freeTree(tree);
    return codeStr;
}
void decompressHeper(string filename, int &pos, string &ext, hashmapF &dump) {
    filename = filename.substr(0, pos);
    ifbitstream input(filename + ext + ".huf");
    ofstream output(filename + "_unc" + ext);
    buildFrequencyMap(filename+ext, true, dump);
}
//
// *This function completes the entire decompression process.  Given the file,
// filename (which should end with ".huf"), (1) extract the header and build
// the frequency map; (2) build an encoding tree from the frequency map; (3)
// using the encoding tree to decode the file.  This function should create a
// compressed file using the following convention.
// If filename = "example.txt.huf", then the uncompressed file should be named
// "example_unc.txt".  The function should return a string version of the
// uncompressed file.  Note: this function should reverse what the compress
// function did.
//
string decompress(string filename) {
    int  pos = filename.find(".huf");
    if (pos >= 0) filename = filename.substr(0, pos);
    pos = filename.find(".");
    string ext = filename.substr(pos, filename.length() - pos);
    filename = filename.substr(0, pos);
    ifbitstream input(filename + ext + ".huf");
    hashmapF dump;
    input >> dump;
    ofstream output(filename + "_unc" + ext);
    HuffmanNode* tree = buildEncodingTree(dump);
    string decodeStr  = decode(input, tree, output);
    output << decodeStr;
    output.close();
    freeTree(tree);
    return decodeStr;  // TO DO: update this return
}