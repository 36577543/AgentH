#pragma once
#include <string>
#include <set>
#include <map>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include "json11.hpp"


struct Config
{
public:
	///< 获取该结构唯一实例
	static Config& instance();

	///< 加载全部配置
	bool loadConfig();

private:
	Config() {}
	virtual ~Config() {}

	log4cplus::Logger logger = log4cplus::Logger::getInstance("main");
};
