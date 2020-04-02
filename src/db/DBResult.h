#pragma once
class DBResult
{
public:
	DBResult() {};
	virtual ~DBResult() {};
	virtual bool isEmpty() = 0;
	virtual bool fetch() = 0;
	virtual uint32 getRowCount() = 0;
	virtual uint32 getFieldsCount() = 0;

	virtual const char * getData(int & len) = 0;
	virtual const char * getData() = 0;

};

