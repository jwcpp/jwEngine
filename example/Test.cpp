#include <string>
#include <stdlib.h>

int main()
{
	std::string s("0");
	s.reserve(20);
	printf("%d\n", s.capacity());
	printf("%x\n", s.data());
	
	s.resize(0);
	s.insert(0, "111111111111111");

	printf("%d\n", s.capacity());
	printf("%x\n", s.data());


	printf("%s\n", s.data());
	system("pause");
	return 0;
}