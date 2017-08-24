#include "HuffmanCoding.h"

#include <functional>
#include <fstream>
#include <iostream>
#include <sstream>

namespace HuffmanCoding
{ 

////////////////Byte////////////////

	// Byte with abbility to address each bit.
	// Used to save Huffman Code efficient to binary file.
	union Byte
	{
		unsigned char byte;
		struct Bits
		{
			unsigned bit0 : 1, bit1 : 1, bit2 : 1, bit3 : 1, bit4 : 1, bit5 : 1, bit6 : 1, bit7 : 1;
		}bits;

		// Set all bits default to zero. 
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


////////////////Encoder::Node////////////////

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



////////////////Encoder////////////////

	 Encoder::Encoder(std::string const& in, InputFlags flag) 
	 {
		 std::string toCompress;
		 if (flag == InputFlags::filename)
		 {
			 try
			 {
				 readRawFile(in, toCompress);
			 }
			 catch (std::exception& e)
			 {
				 throw;
			 }
		 }
		 else if (flag == InputFlags::rawString)
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
		 //prettyPrint(huffQueue.top(),"");
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

	 void Encoder::safeTableToFile(std::string const& filename, int size)
	 {
		 std::ofstream out("key" + filename);
		 if (!out.is_open())
		 {
			 throw std::runtime_error("HuffmanCoding::Encoder::safeTableToFile: can't open file");
		 }
		 else
		 {

			 out << size << std::endl;

			 for (auto it : huffTable)
			 {
				 out << it.first << " " << it.second << "\n";
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
		 float howManyBytes = encodedText.size() / 8.0f;
		 int size = static_cast<int>(ceilf(howManyBytes));

		 try
		 {
			 safeTableToFile(filename, size);
		 }
		 catch (std::exception& e)
		 {
			 throw;
		 }

		 std::vector<Byte*> bytesToStore;
		 bytesToStore.reserve(size);
		 //logMsg(encodedText);
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
				 whichByte++;
				 cnt = -1;
				 break;
			 default:
				 break;
			 }
		 }
		 std::string resStr;
		 for (int i = 0; i< size; i++)
		 {
			 resStr += std::to_string(bytesToStore[i]->bits.bit0);
			 resStr += std::to_string(bytesToStore[i]->bits.bit1);
			 resStr += std::to_string(bytesToStore[i]->bits.bit2);
			 resStr += std::to_string(bytesToStore[i]->bits.bit3);
			 resStr += std::to_string(bytesToStore[i]->bits.bit4);
			 resStr += std::to_string(bytesToStore[i]->bits.bit5);
			 resStr += std::to_string(bytesToStore[i]->bits.bit6);
			 resStr += std::to_string(bytesToStore[i]->bits.bit7);
		 }
	//	 logMsg(resStr);

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

	 std::string Encoder::getEncodedText()
	 {
		 return encodedText;

	 }


////////////////Decoder////////////////

	 Decoder::Decoder(std::string const& in, InputFlags flag)
	 {
		 std::string toDecode;
		 
		 if (flag == InputFlags::filename)
		 {
			 try
			 {
				 int size = readTableFromFile("key" + in);
				 readEncodedFile(in, size, toDecode);
			 }
			 catch (const std::exception&)
			 {
				 throw;
			 }
		 }
		 else if (flag == InputFlags::rawString)
		 {
			 toDecode = in;
		 }
		 decode(toDecode);
	 }

	 int Decoder::readTableFromFile(std::string const& filename)
	 {
		 std::ifstream in(filename);
		 char key;
		 std::string data;
		 int size = 0;
		 if (in.is_open())
		 {

			 in >> size;
			 while (in >> key >> data)
			 {
				 huffTable.insert(std::pair<std::string, char>(data, key));
			 }
		 }
		 else
		 {
			throw std::runtime_error("Error at HuffmanCoding::Decoder::readTableFromFile: can't open file: " + filename);
		 }
		 logMsg("size: " + std::to_string(size));

		 for (auto it : huffTable)
		 {
			 logMsg("Table content: " + it.first  + " " + std::string(1, it.second));
		 }

		 in.close();

		 return size;
	 }

	 void Decoder::readEncodedFile(std::string const& filename, int size, std::string& out)
	 {
		 std::ifstream in(filename, std::ios::in | std::ios::binary);

		 std::vector<Byte*> bytesToRead;
		 bytesToRead.reserve(size);

		 out = "";

		 for (int i = 0; i < size; i++)
		 {
			 bytesToRead.push_back(new Byte());
		 }
	
		 if (in.is_open())
		 {
			 for (int i = 0; i < size; i++)
			 {
				 in.read((char*)bytesToRead[i], sizeof(unsigned char));

				 out += std::to_string(bytesToRead[i]->bits.bit0);
				 out += std::to_string(bytesToRead[i]->bits.bit1);
				 out += std::to_string(bytesToRead[i]->bits.bit2);
				 out += std::to_string(bytesToRead[i]->bits.bit3);
				 out += std::to_string(bytesToRead[i]->bits.bit4);
				 out += std::to_string(bytesToRead[i]->bits.bit5);
				 out += std::to_string(bytesToRead[i]->bits.bit6);
				 out += std::to_string(bytesToRead[i]->bits.bit7);
			 }
		 }
		 else
		 {
			 throw std::runtime_error("Error at HuffmanCoding::Decoder::readEncodedFile: can't open file: " + filename);
		 }

		 for (auto it : bytesToRead)
		 {
			 delete it;
			 it = nullptr;
		 }
	 }

	 void Decoder::decode(std::string const& text)
	 {
		 std::string toFind;
		 for (auto strIt : text)
		 {
			 toFind += strIt;
			 auto huffTableIt = huffTable.find(toFind);
			
			 if (huffTableIt == huffTable.end())
			 {
				 continue;
			 }
			 else 
			 {
				 decodedText += huffTableIt->second;
				 toFind = "";
			 }
			 
			 //throw std::runtime_error("HuffmanCoding::Encoder::translateText(): can't find character in huffTable: " + std::string(1,it));
		 }
	 }

	 std::string Decoder::getDecoded()
	 {
		 return decodedText;
	 }

	 void Decoder::safeDecodedToFile(std::string const& filename)
	 {
		 std::ofstream out(filename);
		 if (!out.is_open())
		 {
			 throw std::runtime_error("HuffmanCoding::Decoder::safeDecodedToFile: can't open file: + filename");
		 }
		 else
		 {
			 out << decodedText << std::endl;	 
			 out.close();
		 }
	 }
}

