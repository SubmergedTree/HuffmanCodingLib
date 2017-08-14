#include "HuffmanCoding.h"

#include <functional>
#include <fstream>
#include <iostream>
#include <sstream>

namespace HuffmanCoding
{
	FileNotFoundException::FileNotFoundException(std::string fileName) : fileName(fileName)
	{}

	const char * FileNotFoundException::what() const throw()
	{
		return std::string("HuffmanCoding error: can't access" + fileName).c_str();
	}



	Encoder::Node::sharedPtr Encoder::Node::newShared(unsigned prevalence, char character)
	{
		return std::make_shared<Node>(character, prevalence, true);
	}

	Encoder::Node::sharedPtr Encoder::Node::newShared(unsigned prevalence)
	{
		return std::make_shared<Node>(char(' '), prevalence, false);
	}

	 Encoder::Node::Node(char character, unsigned int prevalence, bool isLeaf) :
		 character(character), prevalence(prevalence), isLeaf(isLeaf)
	 {
		 left = right = nullptr;
	 }


	 Encoder::Encoder(std::string const& in, Flags flag) 
	 {
		 std::string toCompress;
		 if (flag == Flags::filename)
		 {
			 try
			 {
				 readRawFile(in, toCompress);
			 }
			 catch (FileNotFoundException e)
			 {
				 throw;
			 }
			 
		 }
		 else if (flag == Flags::rawString)
		 {
			 toCompress = in;
		 }
		 std::map<char, unsigned int> countedCharacters;
		 countCharacters(toCompress,countedCharacters);
		 createTree(countedCharacters);
		// createTable();
	 }


	 void Encoder::createTree(std::map<char,unsigned int>& countedCharacters)
	 {
		 auto comp = [](Node::sharedPtr l, Node::sharedPtr r)
		 {
			 return (l->prevalence > r->prevalence);
		 };

		 HuffQueue <decltype(comp)> huffQueue(comp);

		 for (auto it : countedCharacters)
		 {
			 huffQueue.push(Node::newShared(it.first, it.second));
		 }

		 Node::sharedPtr  top, left, right;

		 while (huffQueue.size() > 1)
		 {
			 left = huffQueue.top();
			 huffQueue.pop();

			 right = huffQueue.top();
			 huffQueue.pop();

			 top = Node::newShared(left->prevalence + right->prevalence);
			 top->left = left;
			 top->right = right;
			 huffQueue.push(top);
		 }

	 }

	 void Encoder::createTable(Node::sharedPtr node)
	 {
	 }

	 void Encoder::readRawFile(std::string const& filename, std::string& out)
	 {
		 std::ifstream file(filename);
		 std::stringstream strBuffer;

		 if (file.is_open())
		 {
			 strBuffer << file.rdbuf();
			 file.close();
		 }
		 else
		 {
			 throw FileNotFoundException(filename);
		 }
		 out = strBuffer.str();
	 }

	 void Encoder::countCharacters(std::string& rawStr, std::map<char, unsigned int>& countedCharacters)
	 {
		 for (int i = 0; i < rawStr.length(); i++)
		 {
			 auto it = countedCharacters.find(rawStr[i]);

			 if (it == countedCharacters.end())
			 {
				 countedCharacters.insert(std::pair<char, unsigned int>(rawStr[i], 0));
			 }
			 else
			 {
				 it->second++;
			 }
		 }
	 }

	 void Encoder::printTable()
	 {
	 }

	 void Encoder::safeToFile()
	 {
	 }
}

