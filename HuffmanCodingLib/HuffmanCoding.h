#pragma once

#include <memory>
#include <queue>
#include <vector>
#include <map>
#include <string>
#include <exception>


#include "Logger.h"


namespace HuffmanCoding
{
	// Decide if "in" parameter is a filename or a string which
	// should be compressed.
	enum InputFlags 
	{
		filename,
		rawString
	};

	//enum PrintFlags
	//{
	//	alphabeticalOrder,
	//	//shortestCode,
	//	//longestCode,
	//	unsorted
	//};

	// Encode file or string. 
	class Encoder
	{
	private:

		// Nodes from HuffmanTree.
		struct Node
		{
			typedef std::shared_ptr<Node> sharedPtr;

			// Creates new Node.
			// character: Character to encode.
			// prevalence: Prevalence of a Character
			static sharedPtr newShared(char character, unsigned prevalence);

			// Create new Node.
			// prevalence: Prevalence of a Character
			static sharedPtr newShared(unsigned prevalence);
 
			char character;
			unsigned int prevalence;
			bool isLeaf;

			sharedPtr left, right;

			Node(char character, unsigned int prevalence, bool isLeaf);		
		};

		//ONLY FOR TEST PURPOSE
	/*	void prettyPrint(Node::sharedPtr node, std::string str)
		{
			if (node)
			{
				if (node->isLeaf)
				{
					logMsg(std::string(1, node->character) + " : " + str);
				}
				else
				{
					prettyPrint(node->left, str + "0");
					prettyPrint(node->right, str + "1");
				}
			}

		}*/

		template <typename comp>
		using HuffQueue = std::priority_queue<Node::sharedPtr, std::vector<Node::sharedPtr>, comp>;

		// Stores Huffman codes.
		std::map<char, std::string> huffTable;

		// Huffman encoded Text.
		std::string encodedText;

		// Create a new Huffman Tree.
		// countedCharacters: Occurrence of single character.
		Node::sharedPtr createTree(std::map<char, unsigned int>& countedCharacters);

		// Creates recursively a table from an existing Tree.
		// node: Root of Huffman Tree.
		// res: Binary sequence of Huffman code for an character.
		void createTable(Node::sharedPtr node, std::string res);

		// Reads the file which should be compressed
		// filename: Name of the file.
		// out: Content from file.
		void readRawFile(std::string const& filename, std::string& out);

		// Count prevalence of a specific character.
		// rawStr: text to encode.
		// countedCharacters: result 
		void countCharacters(std::string& rawStr, std::map<char, unsigned int>& countedCharacters);

		// Encodes text to Huffman compressed code.
		// toCompress: text to encode.
		void translateText(std::string const& toCompress);

		// Saves table which contains specific binary code for a character and size of the dataset.
		// Named like the actual compressed file but with prefix "key". 
		// filename: name from file.
		void safeTableToFile(std::string const& filename, int size);
	public:
		Encoder(std::string const& in,InputFlags flag);

		// Print table of Huffman codes to Console.
		void printTable(/*PrintFlags flag*/);

		// Safe encoded data to file.
		// filename: name of file.
		void safeToFile(std::string const& filename);

		// Get Huffman code of a character.
		// Throws an exception if key is not in table.
		// key: character which belongs to one code.
		std::string getFromTable(char key);

		//Get encoded text.
		std::string getEncodedText();
	};


	// Decode file or string.
	class Decoder
	{
	private:
		// Stores Huffman codes.
		std::map<std::string, char> huffTable;

		// decoded text.
		std::string decodedText;

		// Read File with prefix "key".
		// This file stores Huffman codes and number of characters.
		// filename: name of file.
		// returns size of dataset.
		int readTableFromFile(std::string const& filename);

		// Read binary encoded text.
		// filename: name of file.
		// size: size of dataset.
		// out: wil be filled with Huffman code.
		void readEncodedFile(std::string const& filename, int size, std::string& out);

		// Decoding text.
		// text: Encoded text to decode.
		void decode(std::string const& text);

	public:

		Decoder(std::string const& in, InputFlags flag);

		// Return decoded Text.
		std::string getDecoded();

		// Safe decoded text to file.
		// filename: name of file to save.
		void safeDecodedToFile(std::string const& filename);
	};
}
