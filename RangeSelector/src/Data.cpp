#include <string>
#include <vector>

class Sensor {
public:
	std::string name;
	std::vector<float> values;
};

class Data {
public:
	std::vector<Sensor> sensors;
};

