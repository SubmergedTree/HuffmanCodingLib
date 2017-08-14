#pragma once

#include <memory>
#include <queue>
#include <vector>
#include <map>
#include <string>

namespace HuffmanCoding
{
	enum Flags 
	{
		filename,
		rawString
	};

	class Encoder
	{
	private:

		struct Node
		{
			typedef std::shared_ptr<Node> sharedPtr;

			static sharedPtr newShared(unsigned freq, char data);
			static sharedPtr newShared(unsigned freq);

			char character;
			unsigned int prevalence;
			bool isLeaf;

			sharedPtr left, right;

			Node(char character, unsigned int prevalence, bool isLeaf);		
		};

		template <typename comp>
		using HuffQueue = std::priority_queue<Node::sharedPtr, std::vector<Node::sharedPtr>, comp>;

		std::map<char, std::string> huffTable;

		void createTree();
		void createTable(Node::sharedPtr node);

	public:
		Encoder(std::string const& in,Flags flag);

		void printTable();
		void safeToFile();
	};


	class Decoder
	{

	};
}