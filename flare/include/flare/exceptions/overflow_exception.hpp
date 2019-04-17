#ifndef _FLARE_OVERFLOWEXCEPTION_HPP_
#define _FLARE_OVERFLOWEXCEPTION_HPP_

#include <exception>
#include <string>

namespace flare
{
	class OverflowException : public std::exception
	{
		std::string m_Message;

		public:
			OverflowException(std::string msg) : m_Message(msg)
			{

			}

			const std::string message() const
			{
				return m_Message;
			}
	};
}
#endif