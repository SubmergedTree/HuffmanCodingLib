#pragma once

#include <memory>
#include <queue>
#include <vector>
#include <map>
#include <string>
#include <exception>

//TODO: use typedef instead map<char, unsigned>

namespace HuffmanCoding
{
	class FileNotFoundException : public std::exception
	{
	private:
		std::string fileName;
	public:
		explicit FileNotFoundException(std::string fileName);
		virtual const char* what() const throw();
	};

	class NotInTableException : public std::exception
	{
	private:
		char key;
	public:
		explicit NotInTableException(char key);
		virtual const char* what() const throw();
	};

	enum EncoderInputFlags 
	{
		filename,
		rawString
	};

	enum PrintFlags
	{
		alphabeticalOrder,
		shortestCode,
		longestCode,
		unsorted
	};

	class Encoder
	{
	private:

		struct Node
		{
			typedef std::shared_ptr<Node> sharedPtr;

			static sharedPtr newShared(unsigned prevalence, char character);
			static sharedPtr newShared(unsigned prevalence);

			char character;
			unsigned int prevalence;
			bool isLeaf;

			sharedPtr left, right;

			Node(char character, unsigned int prevalence, bool isLeaf);		
		};

		template <typename comp>
		using HuffQueue = std::priority_queue<Node::sharedPtr, std::vector<Node::sharedPtr>, comp>;

		std::map<char, std::string> huffTable;

		Node::sharedPtr createTree(std::map<char, unsigned int>& countedCharacters);
		void createTable(Node::sharedPtr node, std::string res);
		void readRawFile(std::string const& filename, std::string& out);
		void countCharacters(std::string& rawStr, std::map<char, unsigned int>& countedCharacters);

	public:
		Encoder(std::string const& in,EncoderInputFlags flag);

		void printTable(PrintFlags flag);
		void safeToFile();
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