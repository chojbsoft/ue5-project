#include "pch.h"
#include "Logger.h"
#include "Engine.h"

Logger logger;

Logger::Logger()
{
	_logThread = thread([this]
		{
			unique_lock<mutex> lock(_mutex, defer_lock);
			while (!_isExitReq)
			{
				lock.lock();

				_condVar.wait(lock);

				_frontBuf.swap(_backBuf);
				lock.unlock();

				while (!_frontBuf.empty())
				{
					cout << _frontBuf.front() << endl;
					_frontBuf.pop();
				}
			}
		});
}

void Logger::Log(ELogLevel level, const string& msg)
{
	unique_lock<mutex> lock(_mutex);
	_backBuf.push(GetCurrTime() + " [" + LevelToStr(level) + "] " + msg);
	_condVar.notify_all();
}

void Logger::Exit()
{
	LOG(Info, "logThread join");
	_isExitReq = true;
	_condVar.notify_all();
	_logThread.join();

	while(!_backBuf.empty())
	{
		cout << _backBuf.front() << endl;
		_backBuf.pop();
	}
}

string Logger::GetCurrTime()
{
	auto now = chrono::system_clock::now();
	auto toTime = chrono::system_clock::to_time_t(now);

	tm buf;
	localtime_s(&buf, &toTime);
	stringstream timeStream;
	timeStream << put_time(&buf, "%Y-%m-%d %H:%M:%S");
	return timeStream.str();
}

string Logger::LevelToStr(ELogLevel level)
{
	switch (level)
	{
	case ELogLevel::InfoLevel:
		return "INFO";
	case ELogLevel::WarningLevel:
		return "WARNING";
	case ELogLevel::ErrorLevel:
		return "ERROR";
	default:
		return "UNKNOWN";
	}
}