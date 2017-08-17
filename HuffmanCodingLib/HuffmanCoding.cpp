#include "HuffmanCoding.h"

#include <functional>
#include <fstream>
#include <iostream>
#include <sstream>

namespace HuffmanCoding
{ 
	Encoder::Node::sharedPtr Encoder::Node::newShared(char character, unsigned prevalence)
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


	 Encoder::Encoder(std::string const& in, EncoderInputFlags flag) 
	 {
		 std::string toCompress;
		 if (flag == EncoderInputFlags::filename)
		 {
			 try
			 {
				 readRawFile(in, toCompress);
			 }
			 catch (std::exception e)
			 {
				 throw;
			 }
		 }
		 else if (flag == EncoderInputFlags::rawString)
		 {
			 toCompress = in;
		 }

	 
		 std::map<char, unsigned int> countedCharacters;
		 countCharacters(toCompress,countedCharacters);
		 Node::sharedPtr topOfTree = createTree(countedCharacters);
		 createTable(topOfTree,"");	
	 }

	 Encoder::Node::sharedPtr Encoder::createTree(std::map<char,unsigned int>& countedCharacters)
	 {
		 auto comp = [](Node::sharedPtr l, Node::sharedPtr r)
		 {
			 return (l->prevalence > r->prevalence);
		 };

		 HuffQueue <decltype(comp)> huffQueue(comp);

		 for (auto it : countedCharacters)
		 {
			 huffQueue.push(Node::newShared(it.first, it.second));
			// logMsg(std::string(1,it.first) + std::to_string(it.second));
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
		 prettyPrint(huffQueue.top(),"");
		 return huffQueue.top();
	 }

	 void Encoder::createTable(Node::sharedPtr node, std::string res = " ")
	 {
		 if (node)
		 {
			 if (node->isLeaf)
			 {
				 huffTable.insert(std::pair<char, std::string>(node->character, res));
			 }
			 else
			 {
				 createTable(node->left, res + "0");
				 createTable(node->right, res + "1");
			 }
		 }
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
			 std::string errorMsg("Error at HuffmanCoding::Encoder::readRawFile can't open file: ");
			 errorMsg += filename;
			 throw std::runtime_error(errorMsg.c_str());
		 }
		 out = strBuffer.str();
	 }

	 void Encoder::countCharacters(std::string& rawStr, std::map<char, unsigned int>& countedCharacters)
	 {
		 for (unsigned i = 0; i < static_cast<unsigned>(rawStr.length()); i++)
		 {
			 auto it = countedCharacters.find(rawStr[i]);

			 if (it == countedCharacters.end())
			 {
				 countedCharacters.insert(std::pair<char, unsigned int>(rawStr[i], 1));
			 }
			 else
			 {
				 it->second++;
			 }
		 }
	 }

	 void Encoder::printTable(/*PrintFlags flag*/)
	 {
		 //auto localTable = this->huffTable;

		 //switch (flag)
		 //{
		 //case PrintFlags::unsorted:
			// break;
		 //case PrintFlags::alphabeticalOrder:

			// break;
		 ////case PrintFlags::longestCode:
			//// break;
		 ////case PrintFlags::shortestCode:
			//// break;
		 //}

		 std::for_each(huffTable.begin(), huffTable.end(), [](std::pair<char, std::string>& pair)
		 {
			 std::cout << pair.first << " : " << pair.second << std::endl;
		 });
	 }

	 void Encoder::safeToFile(std::string filename)
	 {
	 }

	 std::string Encoder::getFromTable(char key)
	 {
		 auto it = huffTable.find(key);
		 if (it == huffTable.end())
		 {
			 std::string errorMsg("Error at HuffmanCoding::Encoder::getFromTable: not in table: ");
			 errorMsg += key,
			 throw std::runtime_error(errorMsg);
		 }
		 else
		 {
			 return it->second;
		 }
	 }
}

