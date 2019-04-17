#include "flare/block_reader.hpp"
#include "flare/exceptions/overflow_exception.hpp"

#include "flare_math/mat2d.hpp"
#include "flare_math/vec2d.hpp"
#include "flare_math/color.hpp"

using namespace flare;

BlockReader::BlockReader(unsigned char* data, unsigned int length) :
    BinaryReader(data, length, BinaryReader::LittleEndian)
{
}

BlockReader::BlockReader(int type, unsigned char* data, unsigned int length) :
    BinaryReader(data, length, BinaryReader::LittleEndian),
    m_BlockType(type)
{
}

void BlockReader::read(Mat2D& mat)
{
	float arr[6];
	readFloat32Array(arr, 6);
	mat[0] = arr[0];
	mat[1] = arr[1];
	mat[2] = arr[2];
	mat[3] = arr[3];
	mat[4] = arr[4];
	mat[5] = arr[5];
}

void BlockReader::read(Vec2D& vec)
{
	float arr[2];
	readFloat32Array(arr, 2);
	vec[0] = arr[0];
	vec[1] = arr[1];
}

void BlockReader::read(Color& color)
{
	float arr[4];
	readFloat32Array(arr, 4);
	color[0] = arr[0];
	color[1] = arr[1];
	color[2] = arr[2];
	color[3] = arr[3];
}

BlockReader* BlockReader::readNextBlock()
{
	if (m_DataPosition == m_DataLength)
	{
		return nullptr;
	}
	unsigned char blockType = readUint8();
	unsigned int length = readUint32();

	if (m_DataPosition + length > m_DataLength)
	{
		throw OverflowException("Block length is invalid and would cause overflow in BlockReader::readNextBlock.");
	}
	unsigned char* blockData = m_Data + m_DataPosition;

	m_DataPosition += length;
	return new BlockReader((int)blockType, blockData, length);
}