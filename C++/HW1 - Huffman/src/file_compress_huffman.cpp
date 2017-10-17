#include <fstream>
#include <string>
#include <iostream>
#include <queue>
#include <vector>
#include <cstdlib>
#include "file_compress_huffman.hpp"
using namespace std;


void make_codes(code_tree * current, string codes[])
{
    string parent_code = current->code;        
    if (current->left != 0)
    {
        (current->left)->code = parent_code + '0';
        make_codes(current->left, codes);
    }
    if (current->right != 0)
    {    
        (current->right)->code = parent_code + '1';
        make_codes(current->right, codes);
    }
    if (current->right == 0 && current->left == 0)
    {
        codes[(size_t)(unsigned char)(current->character)] = current->code;
    }
}

void codes_output(code_tree * current, ostream & output_file){
    if (current->left == 0 && current->right == 0)
    {    
        output_file << (size_t)(unsigned char)(current->character) << " " << current->code << '\n';
    }
    if (current->left != 0)
    {
        codes_output (current->left,output_file);
    }
    if (current->right != 0)
    {
        codes_output (current->right,output_file);
    }
}





void  file_compress_huffman (string const & input_file_path, string const & output_file_path)
{

    ifstream input_file (input_file_path.c_str());
	if (!input_file.is_open())
	{		
        cout << "Incorrect input file" << endl; 
		exit(1);
	}

    ofstream output_file (output_file_path.c_str());

    priority_queue<code_tree*, vector<code_tree*>,compare> tree_queue;
    
    int     char_map[256];
    for (size_t i=0; i<256; ++i)
    {
        char_map[i]=0;
    }

    char character;
    while (input_file.get(character))
    {
        char_map[(size_t)(unsigned char)character]++;
    }

    for (size_t i=0; i<256; ++i)
    {
        if (char_map[i]!=0)
        {
            code_tree * next = new code_tree(i, char_map[i]);
            tree_queue.push(next);  
        }
    }

    size_t alphabet_size = tree_queue.size();
    output_file << alphabet_size << '\n';

    size_t memory = sizeof(char_map) + sizeof(tree_queue);

    if (alphabet_size == 1)
    {
        code_tree * single = tree_queue.top();
        output_file << size_t(single->character) << " 0\n"; 
        input_file.close();
        input_file.open (input_file_path.c_str(), ios::binary);

        while (input_file.get(character))    
        {
            output_file << '0';
        } 
        delete single;
    }
    else
    {
        while (tree_queue.size() > 1)
        {
            code_tree * x = tree_queue.top();
            tree_queue.pop();    
            code_tree * y = tree_queue.top();
            tree_queue.pop();
            code_tree * next = new code_tree(-1 , x->priority + y->priority, x, y);
            tree_queue.push(next);
        }    
        if (tree_queue.size() == 0)
        {
            return;
        }
        code_tree *huffman_tree = 0;
        huffman_tree = tree_queue.top();

        string symbols_codes[256];

        make_codes (huffman_tree, symbols_codes);
        codes_output (huffman_tree, output_file);
        input_file.close();
        input_file.open (input_file_path.c_str(), ios::binary);

        while (input_file.get(character))    
        {
            output_file << symbols_codes[(size_t)(unsigned char)character];
        } 

        memory += sizeof(huffman_tree) + sizeof(symbols_codes);
        delete huffman_tree;

    }


    input_file.close();
    output_file.close();

    ifstream input_file_size_check(input_file_path.c_str());
    input_file_size_check.seekg(0, ios::end);
    cout << input_file_size_check.tellg() << endl;
    input_file_size_check.close();

    ifstream output_file_size_check(output_file_path.c_str());
    output_file_size_check.seekg(0, ios::end);
    cout << output_file_size_check.tellg() << endl;
    output_file_size_check.close();

    cout << memory << endl;



}
  

