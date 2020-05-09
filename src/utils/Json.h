#pragma once

#include <string>
#include <vector>
#include <map>

namespace jwEngine
{
	enum eJOSN_TYPE
	{
		eJ_INT,   // int
		eJ_FLOAT, // float
		eJ_NULL,  // null
		eJ_STRING,// string
		eJ_BOOL,  // bool
		eJ_ARRAY, // array
		eJ_OBJ,   // obj
	};
	class JBaseObj
	{
	public:
		JBaseObj();
		virtual ~JBaseObj();

		int type = eJ_NULL; // eJOSN_TYPE

		virtual void write(std::string & str, bool layout = true, int depth = 0) = 0;
	};

	// base value
	class JValue: public JBaseObj
	{
	public:
		JValue();
		~JValue();
		void setInt(int v);
		void setFloat(float v);
		void setStr(const char * v);
		void setBool(bool v);
		void setNull();

		virtual void write(std::string & str, bool layout = true, int depth = 0);

	public:
		union
		{
			int			iv;
			float		fv;
			bool		bv;
		};
		std::string sv;
	};

	class JSonObj;
	class JsonReader;

	// array type
	class JArray: public JBaseObj
	{
		friend JsonReader;
	public:
		JArray();
		~JArray();

		void pushInt(int v);
		void pushFloat(float v);
		void pushStr(const char * v);
		void pushBool(bool v);
		void pushNull();

		int getInt(int idx);
		float getFloat(int idx);
		bool getBool(int idx);
		const char * getStr(int idx);
		bool isNull(int idx);

		void setInt(int idx, int v);
		void setFloat(int idx, float v);
		void setStr(int idx, const char * v);
		void setBool(int idx, bool v);
		void setNull(int idx);

		void remove(int idx);

		int size();
		JArray * createArray();
		JSonObj * createObj();
		JBaseObj * getJsonObj(int idx);

		virtual void write(std::string & str, bool layout = true, int depth = 0);
	private:
		std::vector<JBaseObj *> items;
	};

	class JSonObj : public JBaseObj
	{
		friend JsonReader;
	public:
		JSonObj();
		~JSonObj();

		int getInt(std::string key);
		float getFloat(std::string key);
		bool getBool(std::string key);
		const char * getStr(std::string key);
		bool isNull(std::string key);

		void setInt(std::string key, int v);
		void setFloat(std::string key, float v);
		void setStr(std::string key, const char * v);
		void setBool(std::string key, bool v);
		void setNull(std::string key);

		void remove(std::string key);

		JArray * createArray(std::string key);
		JSonObj * createObj(std::string key);
		JBaseObj * getJsonObj(std::string key);

		virtual void write(std::string & str, bool layout = true, int depth = 0);

	private:
		std::map<std::string, JBaseObj *> maps;
	};
}