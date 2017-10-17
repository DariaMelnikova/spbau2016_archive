#include <fstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include "file_decompress_huffman.hpp"
using namespace std;

void build_codes_tree(code_tree* root, istream & input_file)
{
    size_t alphabet_size;
    input_file >> alphabet_size;
    string empt;
    getline(input_file, empt);
    for (size_t i = 0; i < alphabet_size; ++i)
    {
        size_t character;
        string code;
        input_file >> character;
        getline(input_file,code);
        code_tree *iter = root;      
        for (size_t j = 1; j < code.length(); ++j)
        {
            if (code[j] == '0')
            {
                if (iter->left != NULL)
                {
                    iter = iter->left;
                }
                else 
                {
                    code_tree *next = new code_tree();
                    iter->left = next;
                    iter = iter->left;
                }
            }
            if (code[j] == '1')
            {
                if (iter->right != NULL)
                {
                    iter = iter->right;
                }
                else 
                {
                    code_tree *next = new code_tree();
                    iter->right = next;
                    iter = iter->right;
                }    
            }
        }
        iter->character = character;
    }
}

void uncode_file(code_tree * root, istream & input_file, ostream & output_file)
{
    char character;
    while (input_file.get(character))
    {    
        code_tree * iter = root;
        while (iter->left != NULL && iter->right != NULL)
        {
            if (character == '0')
            {
                iter = iter->left;
            }
            if (character == '1')
            { 
                iter = iter->right;
            }
            if (iter->left != NULL && iter->right != NULL)            
            {            
                input_file.get(character);
            }
        }
        if (iter->left != NULL && iter->right ==  NULL && character == '0')
        {
            iter = iter->left;
        }
        if (iter->right != NULL && iter->left ==  NULL && character == '1')
        {
            iter = iter->right;
        }
        output_file << iter->character;
    }
}


void  file_decompress_huffman (string const & input_file_path, string const & output_file_path)
{
    code_tree *root = new code_tree();

    ifstream input_file(input_file_path.c_str(),ios::binary);
	if (!input_file.is_open())
	{		
        cout << "Incorrect input file" << endl; 
		exit(1);
	}

    ofstream output_file(output_file_path.c_str());

    build_codes_tree(root,input_file);
    uncode_file(root, input_file, output_file);
    

    input_file.close();

    ifstream in_file_size_check(input_file_path.c_str());
    in_file_size_check.seekg(0, ios::end);
    cout << in_file_size_check.tellg() << endl;
    in_file_size_check.close();

    output_file.close();

    ifstream out_file_size_check(output_file_path.c_str());
    out_file_size_check.seekg(0, ios::end);
    cout << out_file_size_check.tellg() << endl;
    out_file_size_check.close();

    cout << sizeof(*root) << endl;
    delete root;
}
