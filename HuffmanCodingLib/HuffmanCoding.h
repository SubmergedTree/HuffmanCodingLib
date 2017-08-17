#pragma once

#include <memory>
#include <queue>
#include <vector>
#include <map>
#include <string>
#include <exception>


#include "Logger.h"

//TODO: use typedef instead map<char, unsigned>

namespace HuffmanCoding
{
	enum EncoderInputFlags 
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

	class Encoder
	{
	private:

		struct Node
		{
			typedef std::shared_ptr<Node> sharedPtr;

			static sharedPtr newShared(char character, unsigned prevalence);
			static sharedPtr newShared(unsigned prevalence);

			char character;
			unsigned int prevalence;
			bool isLeaf;

			sharedPtr left, right;

			Node(char character, unsigned int prevalence, bool isLeaf);		
		};

		//ONLY FOR TEST PURPOSE
		void prettyPrint(Node::sharedPtr node, std::string str)
		{
		/*	if (node)
			{

				if (node->isLeaf)
				{
					logMsg(std::string(1, node->character) + " : " + str);
				}

				prettyPrint(node->left, str + "0");
				prettyPrint(node->right, str + "1");
			}*/

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

		}

		template <typename comp>
		using HuffQueue = std::priority_queue<Node::sharedPtr, std::vector<Node::sharedPtr>, comp>;

		std::map<char, std::string> huffTable;

		Node::sharedPtr createTree(std::map<char, unsigned int>& countedCharacters);
		void createTable(Node::sharedPtr node, std::string res);
		void readRawFile(std::string const& filename, std::string& out);
		void countCharacters(std::string& rawStr, std::map<char, unsigned int>& countedCharacters);

	public:
		Encoder(std::string const& in,EncoderInputFlags flag);

		void printTable(/*PrintFlags flag*/);
		void safeToFile(std::string filename);
		std::string getFromTable(char key);
	};


	class Decoder
	{
	private:
		std::map<char, std::string> huffTable;

		void readFromFile();
	public:
		Decoder(std::string filename);

	};
}