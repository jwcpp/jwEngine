#include "AutoPacket.h"

#include "NetPacket.h"
#include "test.pb.h"


int main()
{
	// ------------------------send msg------------------------

	Family family;
	family.set_name("i family");

	Role * role1 = family.add_roles();
	role1->set_age(40);
	role1->set_name("c++");
	
	Role* role2 = family.add_roles();
	role2->set_age(10);
	role2->set_name("go");

	AutoPacket<NetPacket> apk;
	NetPacket * pack = apk.writeProto<Family>(family);
	// conn->sendMsg(1, pack);

	// -----------------------read msg--------------------

	Family readFamily;
	readProto<NetPacket, Family>(pack, readFamily);

	printf("%s\n", readFamily.name().c_str());
	for (int i = 0; i < readFamily.roles_size(); ++i)
	{
		const Role role = readFamily.roles(i);
		printf("%d\n", role.age());
		printf("%s\n", role.name().c_str());
	}

	return 0;
}