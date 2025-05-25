#pragma once


class Logger
{
public:
	enum class ELogLevel
	{
		InfoLevel,
		WarningLevel,
		ErrorLevel
	};

public:
	Logger();
	void Log(ELogLevel level, const string& msg);
	void Exit();

private:
	string GetCurrTime();
	string LevelToStr(ELogLevel level);

private:
	thread _logThread;
	bool _isExitReq = false;

private:
	queue<string> _frontBuf;
	queue<string> _backBuf;
	mutex _mutex;
	condition_variable _condVar;
};

extern Logger logger;
#define LOG(level, msg) logger.Log(level, msg)
#define Info Logger::ELogLevel::InfoLevel
#define Warning Logger::ELogLevel::WarningLevel
#define Error Logger::ELogLevel::ErrorLevel