#ifndef _FLARE_BLOCKREADER_HPP_
#define _FLARE_BLOCKREADER_HPP_

#include "binary_reader.hpp"
#include <string>

namespace flare
{
	class Mat2D;
	class Vec2D;
	class Color;
	
	class BlockReader;

	class BlockReader : public BinaryReader
	{
	private:
		int m_BlockType;

	public:
		BlockReader(unsigned char* data, unsigned int length);
		BlockReader(int type, unsigned char* data, unsigned int length);
		BlockReader* readNextBlock();

		template <typename T> T blockType() const { return static_cast<T>(m_BlockType); }
		int blockType() const { return m_BlockType; }
		void read(Mat2D& mat);
		void read(Vec2D& vec);
		void read(Color& vec);
		void close() { delete this; }
	};

} //namespace flare
#endif