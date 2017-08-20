#include "HuffmanCoding.h"

#include <functional>
#include <fstream>
#include <iostream>
#include <sstream>

namespace HuffmanCoding
{ 
	union Byte
	{
		unsigned char byte;
		struct Bits
		{
			unsigned bit0 : 1, bit1 : 1, bit2 : 1, bit3 : 1, bit4 : 1, bit5 : 1, bit6 : 1, bit7 : 1;
		}bits;

		Byte()
		{
			bits.bit0 = 0;
			bits.bit1 = 0;
			bits.bit2 = 0;
			bits.bit3 = 0;
			bits.bit4 = 0;
			bits.bit5 = 0;
			bits.bit6 = 0;
			bits.bit7 = 0;
		}
	};



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
		 translateText(toCompress);
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

	 //TODO: use getfromTable 
	 void Encoder::translateText(std::string const& toCompress)
	 {
		 for (auto it : toCompress)
		 {
			 auto huffIt = huffTable.find(it);
			 if (huffIt == huffTable.end())
			 {
				 throw std::runtime_error("HuffmanCoding::Encoder::translateText(): can't find character in huffTable");
			 }
			 else
			 {
				 encodedText += huffIt->second;
			 }
		 }
	 }

	 void Encoder::safeTableToFile(std::string const& filename)
	 {
		 std::ofstream out("key" + filename);
		 if (!out.is_open())
		 {
			 throw std::runtime_error("HuffmanCoding::Encoder::safeTableToFile: can't open file");
		 }
		 else
		 {
			 for (auto it : huffTable)
			 {
				 out << it.first << " / " << it.second << "\n";
			 }
			 out.close();
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

		 std::for_each(huffTable.begin(), huffTable.end(), [](const std::pair<char, std::string>& pair)
		 {
			std::cout << pair.first << " : " << pair.second << std::endl;
		 });
	 }


	 void Encoder::safeToFile(std::string const& filename)
	 {
		 try
		 {
			 safeTableToFile(filename);
		 }
		 catch (std::exception& e)
		 {
			 throw;
		 }

		 float howManyBytes = encodedText.size() / 8.0f;
		 int size = static_cast<int>(ceilf(howManyBytes));

		 std::vector<Byte*> bytesToStore;
		 bytesToStore.reserve(size);

		 for (int i = 0; i < size; i++)
		 {
			 bytesToStore.push_back(new Byte());
		 }

		 for (int i = 0, cnt = 0, whichByte = 0; i < encodedText.size(); i++, cnt++)
		 {
			 switch (cnt)
			 {
			 case 0:
				 bytesToStore[whichByte]->bits.bit0 = static_cast<unsigned>(encodedText[i]);
				 break;
			 case 1:
				 bytesToStore[whichByte]->bits.bit1 = static_cast<unsigned>(encodedText[i]);
				 break;
			 case 2:
				 bytesToStore[whichByte]->bits.bit2 = static_cast<unsigned>(encodedText[i]);
				 break;
			 case 3:
				 bytesToStore[whichByte]->bits.bit3 = static_cast<unsigned>(encodedText[i]);
				 break;
			 case 4:
				 bytesToStore[whichByte]->bits.bit4 = static_cast<unsigned>(encodedText[i]);
				 break;
			 case 5:
				 bytesToStore[whichByte]->bits.bit5 = static_cast<unsigned>(encodedText[i]);
				 break;
			 case 6:
				 bytesToStore[whichByte]->bits.bit6 = static_cast<unsigned>(encodedText[i]);
				 break;
			 case 7:
				 bytesToStore[whichByte]->bits.bit7 = static_cast<unsigned>(encodedText[i]);
				 break;
			 default:
				 break;
			 }
		 }

		 std::ofstream out(filename.c_str(), std::ios::out | std::ios::binary);
		 if (!out.is_open())
		 {
			 throw std::runtime_error("HuffmanCoding::Encoder::safeToFile: can't open file");
		 }

		 for (int i = 0; i < size; i++)
		 {
			 out.write((char*)bytesToStore[i], sizeof(unsigned char));
		 }

		 for (auto it : bytesToStore)
		 {
			 delete it;
			 it = nullptr;
		 }
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

