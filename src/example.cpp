#include <iostream>
#include "jserialize.hpp"

using namespace jserialize;

struct People
{
public:
	People() 
	{
	}
	void SetAgeUInt(uint32_t value){ age = value; }
	void SetMoneyUInt64(uint64_t value) { money = value; }
	void SetSexMan(bool value) { isMan = value; }
	void SetWeight(float value) { weight = value; }
	void SetHeight(double value) { height = value; }
	void SetName(std::string &value) { name = value; }
	void SetHobby(const std::vector<std::string> &value){ hobby = value; }

	GEN_JFLOW_SERIALIZE_FRIEND(People);

private:
	uint32_t age;
	uint64_t money;
	float weight;
	double height;
	bool isMan;
	std::string name;
	std::vector<std::string> hobby;
};

template<class SerializeType>
void Serialize(SerializeType &serialize, People &obj)
{
	serialize	& GEN_JFLOW_SERIALIZE_PAIR("age", obj.age)
		& GEN_JFLOW_SERIALIZE_PAIR("money", obj.money)
		& GEN_JFLOW_SERIALIZE_PAIR("weight", obj.weight)
		& GEN_JFLOW_SERIALIZE_PAIR("height", obj.height)
		& GEN_JFLOW_SERIALIZE_PAIR("man", obj.isMan)
		& GEN_JFLOW_SERIALIZE_PAIR("name", obj.name)
		& GEN_JFLOW_SERIALIZE_PAIR("hobby", obj.hobby);
}

struct ClassRoom
{
	std::string number;
	People teacher;
	std::list<People> students;
};

template<class SerializeType>
void Serialize(SerializeType &serialize, ClassRoom &obj)
{
	serialize	& GEN_JFLOW_SERIALIZE_PAIR("number", obj.number)
		& GEN_JFLOW_SERIALIZE_PAIR("teacher", obj.teacher)
		& GEN_JFLOW_SERIALIZE_PAIR("students", obj.students);
}

int main()
{
	std::string name = "Tom";
	std::vector<std::string> hobby;
	hobby.push_back("c++");
	hobby.push_back("game");
	hobby.push_back("football");
	float weight = 80.01f;
	double height = 176.24;

	People people;
	people.SetAgeUInt(27);
	people.SetMoneyUInt64(60000);
	people.SetWeight(weight);
	people.SetHeight(height);
	people.SetSexMan(true);
	people.SetName(name);
	people.SetHobby(hobby);

	//序列化people到json
	std::string jsonStr = JSerialize::ToString(people);
	std::cout << jsonStr << std::endl;

	//反序列化json到people
	People tmpPeople;
	JSerialize::FromString(jsonStr, tmpPeople);

	//序列化tmpPeople到json比较
	jsonStr = JSerialize::ToString(tmpPeople);
	std::cout << jsonStr << std::endl;

	//嵌套测试
	ClassRoom classRoom;
	classRoom.number = "ClassRoom_123";
	name = "teacher Zhang";
	people.SetName(name);
	classRoom.teacher = people;
	name = "student";
	people.SetName(name);
	classRoom.students.push_back(people);
	classRoom.students.push_back(people);
	classRoom.students.push_back(people);
	classRoom.students.push_back(people);

	//序列化classRoom到json
	jsonStr = JSerialize::ToString(classRoom);
	std::cout << jsonStr << std::endl;

	//反序列化json到tempClassRoom
	ClassRoom tmpClassRoom;
	JSerialize::FromString(jsonStr, tmpClassRoom);

	//序列化tmpClassRoom到json比较
	jsonStr = JSerialize::ToString(tmpClassRoom);
	std::cout << jsonStr << std::endl;

	return 0;
}