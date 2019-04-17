#ifndef _FLARE_BINARYREADER_HPP_
#define _FLARE_BINARYREADER_HPP_

#include <string>

namespace flare
{
	class BinaryReader
	{
	public:
		enum Endian
		{
			BigEndian,
			LittleEndian
		};

	protected:
		unsigned char* m_Data;
		unsigned int m_DataLength;
		unsigned int m_DataPosition;
		bool m_SwapEndianness;

	public:
		BinaryReader(unsigned char* data, unsigned int length, Endian endianness = LittleEndian);

		unsigned char readUint8();
		unsigned int read(unsigned char* bytes, unsigned int length);
		void readFloat32Array(float* numbers, unsigned int length);
		void readUint16Array(unsigned short* numbers, unsigned int length);
		int readInt32();
		unsigned int readUint32();
		float readFloat32();
		double readFloat64();
		short readInt16();
		unsigned short readUint16();
		std::string readString();
		unsigned long readUint64();
	};
} //namespace flare
#endif