#pragma once

#pragma warning(push, 1)
#include <Windows.h>
#pragma warning(pop)

class NamedEvent
{
HANDLE m_eventHandle;
public:
	NamedEvent(const std::string& name)
	{
		m_eventHandle = CreateEventA(NULL, FALSE, FALSE, name.c_str());
		if (m_eventHandle == NULL || m_eventHandle == INVALID_HANDLE_VALUE)
		{
			int errorcode = GetLastError();
			throw std::runtime_error("Cannot create named event: ");
		}
	}

	~NamedEvent()
	{
		CloseHandle(m_eventHandle);
	}

	bool wait(int milliseconds = INFINITE)
	{
		return WaitForSingleObject(m_eventHandle, milliseconds) == 0;
	}

	bool set()
	{
		return SetEvent(m_eventHandle) != 0;
	}
};

class ScopedNamedEvent
{
	NamedEvent m_namedEventStart;
	NamedEvent m_namedEventStop;

public:
	ScopedNamedEvent(const std::string& eventStart, const std::string& eventStop)
		: m_namedEventStart(eventStart), m_namedEventStop(eventStop)
	{
		m_namedEventStart.set();
	}

	~ScopedNamedEvent()
	{
		m_namedEventStop.set();
	}
};