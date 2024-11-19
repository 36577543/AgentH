#include "Config.h"
#include "json11.hpp"
#include <fstream>
#include <sstream>
using namespace json11;
using std::string;

Config& Config::instance()
{
	static Config _config;
	return _config;
}

bool Config::loadConfig()
{
	std::ifstream ifs("app-half.conf");
	std::ostringstream ostr;
	ostr << ifs.rdbuf();

	string err;
	auto json = Json::parse(ostr.str(), err);
	if (!err.empty())
	{
		LOG4CPLUS_ERROR(logger, "loadConfig " << err);
		return false;
	}

	auto addr = json["Login"]["httpAddr"].array_items();
	return true;
}
