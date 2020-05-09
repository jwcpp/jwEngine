#pragma once

#include <string>

namespace jwEngine
{
	class JBaseObj;
	class JsonReader;
	class JsonBuff
	{
		friend JsonReader;
	public:
		JsonBuff(const char *str) { p = (char *)str; }
		~JsonBuff() { if (obj != nullptr) { delete obj; obj = nullptr; } }

		// delete function
		JsonBuff(JsonBuff &) = delete;
		void operator =(JsonBuff &) = delete;

		const char *skip() { while (p && *p && (unsigned char)*p <= 32) p++; return p; }
		const char *curr() { return p; }
		const char *go(int len = 1) { p += len; return p; }
	private:
		char * p;
		JBaseObj * obj = nullptr;
	};

	class JsonReader
	{
	public:

		JsonReader & operator | (JsonReader & r) {
			return *this;
		}

		//read
		static JBaseObj * parser(JsonBuff * buff);
		static std::string getError(JsonBuff * buff);

		static std::string write(JBaseObj * obj,bool layout = true);
	private:
		static bool readStr(std::string & outstr, JsonBuff * buff);
		static JBaseObj * readBase(JsonBuff * buff);
		static JBaseObj * readArray(JsonBuff * buff);
		static JBaseObj * readObj(JsonBuff * buff);
		static JBaseObj * reader(JsonBuff * buff);
	};
}