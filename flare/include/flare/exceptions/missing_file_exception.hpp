#ifndef _FLARE_MISSINGFILEEXCEPTION_HPP_
#define _FLARE_MISSINGFILEEXCEPTION_HPP_

#include <exception>
#include <string>

namespace flare
{
	class MissingFileException : public std::exception
	{
		std::string m_Message;
		std::string m_Filename;
		public:
			MissingFileException(const std::string& msg, const std::string& filename) : m_Message(msg), m_Filename(filename)
			{

			}

			const std::string& message() const
			{
				return m_Message;
			}

			const std::string& filename() const
			{
				return m_Filename;
			}
	};
}
#endif