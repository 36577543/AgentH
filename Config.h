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
	///< ��ȡ�ýṹΨһʵ��
	static Config& instance();

	///< ����ȫ������
	bool loadConfig();

private:
	Config() {}
	virtual ~Config() {}

	log4cplus::Logger logger = log4cplus::Logger::getInstance("main");
};
