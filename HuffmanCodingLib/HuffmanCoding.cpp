#include "HuffmanCoding.h"


namespace HuffmanCoding
{
	Encoder::Node::sharedPtr Encoder::Node::newShared(unsigned freq, char data)
	{
		return std::make_shared<Node>(data, freq, true);
	}

	Encoder::Node::sharedPtr Encoder::Node::newShared(unsigned freq)
	{
		return std::make_shared<Node>(char(' '), freq, false);
	}

	 Encoder::Node::Node(char character, unsigned int prevalence, bool isLeaf) :
		 character(character), prevalence(prevalence), isLeaf(isLeaf)
	 {
		 left = right = nullptr;
	 }


	 void Encoder::createTree()
	 {
	 }

	 void Encoder::createTable(Node::sharedPtr node)
	 {
	 }

	 Encoder::Encoder(std::string const& in, Flags flag)
	 {
	 }

	 void Encoder::printTable()
	 {
	 }

	 void Encoder::safeToFile()
	 {
	 }
	 std::string Encoder::getEncodedText()
	 {
		 return std::string();
	 }
}

