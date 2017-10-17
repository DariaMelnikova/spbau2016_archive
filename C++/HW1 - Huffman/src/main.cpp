#include "file_compress_huffman.hpp"
#include "file_decompress_huffman.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;


int main (int argc, char* argv[])
{

    
    string input_file_path;
    string output_file_path;
    short is_compression_mode = -1;
	if (argc != 6)
	{
		cout << "Wrong input" << endl;
		exit(1);
	}

    for (int i = 0; i < argc; ++i) 
    {
        if (argv[i] == string("-c")) 
        {        
            is_compression_mode = 1;
            continue;
        }
        if (argv[i] == string("-u")) 
        {
            is_compression_mode = 0;
            continue;
        }
        if ((argv[i] == string("--file") || argv[i] == string("-f")) && i + 1 < argc) 
        {
            input_file_path = argv[++i];
            continue;
        }
        if ((argv[i] == string("--output") ||argv[i] == string("-o")) && i + 1 < argc) 
        {
            output_file_path = argv[++i];
            continue;
        }
    }
    if (is_compression_mode == -1 || input_file_path == "" || output_file_path == "") 
    {
        cout << "Wrong input" << endl;
        return 1;
    }

    if (is_compression_mode)
    {
        file_compress_huffman(input_file_path, output_file_path);
    } 
    else
    {  
        file_decompress_huffman(input_file_path, output_file_path);
    }
    return 0;
}
