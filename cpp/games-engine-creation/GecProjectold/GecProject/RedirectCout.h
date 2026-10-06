/*
	RedirectCout.h
	KD: redirects cout and cerr to the output pane in Visual Studio
	Last updated: 19/08/25
*/

#include <iostream>
#include <cassert>

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN             
#define VC_EXTRALEAN
#define NO_MIN_MAX
#define NOMINMAX

#include <windows.h>

class outbuf : public std::streambuf {
public:
	static const size_t KD_bufSize{ 2048 };
private:	
	char buffer[KD_bufSize]{ 0 };
	int num{ 0 };
public:
	outbuf() {
		// no buffering, overflow on every char
		setp(0, 0);
	}

	virtual std::streamsize xsputn(const char* msg, std::streamsize count)
	{		
		std::string s(msg, (unsigned)count);

		OutputDebugStringA(s.c_str());

		num = 0;
		buffer[num] = 0;
		return count;
	}

	virtual int_type overflow(int_type ch)
	{
		buffer[num] = traits_type::to_char_type(ch);
		num++;
		assert(num < KD_bufSize);
		buffer[num] = 0;
		if (ch == '\n')
		{
			OutputDebugStringA(buffer);
			num = 0;
			buffer[num] = 0;
			return ch;
		}

		OutputDebugStringA(buffer);
		num = 0;
		buffer[num] = 0;

		return ch;
	}
};

class outbuferr : public std::streambuf {
private:
	char buffer[outbuf::KD_bufSize] = { 0 };
	int num{ 0 };
public:
	outbuferr() {
		// no buffering, overflow on every char
		setp(0, 0);
	}

	virtual std::streamsize xsputn(const char* msg, std::streamsize count) {

		std::string s(msg, (unsigned int)count);
		
		OutputDebugStringA(buffer);

		num = 0;
		buffer[num] = 0;
		return count;
	}
	virtual int_type overflow(int_type ch)
	{
		buffer[num] = traits_type::to_char_type(ch);
		num++;
		assert(num < outbuf::KD_bufSize);

		if (ch == '\n')
		{
			buffer[num] = 0;
			OutputDebugStringA(buffer);
			num = 0;
			buffer[num] = 0;
			return ch;
		}

		OutputDebugStringA(buffer);
		num = 0;
		buffer[num] = 0;

		return ch;
	}
};