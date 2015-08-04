#include "iostream"
#include "Tracer.h"

#ifndef NDEBUG  //如果不是调试模式，忽略

bool Tracer::_bReady = false;

Tracer::Tracer()
{
	_bReady = true;
}

Tracer::~Tracer()
{
	_bReady = false;
	Dump();
}

void Tracer::Add(void*p, const char*file, long line)
{
	if (_lockCount > 0)
		return;
	Tracer::Lock lock(*this);
	_mapEntry[p] = Entry(file, line);
}

void Tracer::Remove(void* p)
{
	if (_lockCount > 0)
		return;
	Tracer::Lock lock(*this);
	std::map<void*, Entry>::iterator it;
	it = _mapEntry.find(p);
	if (it != _mapEntry.end())
	{
		_mapEntry.erase(it);
	}
}

void Tracer::Dump()
{
	if (_mapEntry.size()>0)
	{
		std::cout << "内存泄漏:" ;
		std::map<void*, Entry>::iterator it;
		for (it = _mapEntry.begin(); it != _mapEntry.end(); ++it)
		{
			const char* file = it->second.File();
			long line = it->second.Line();
			int addr = reinterpret_cast<int>(it->first);
			std::cout <<"0x"<<std::hex <<addr<<":"<<file <<",line:"<< line << std::endl;
		}
		std::cout<< std::endl;
	}
}

Tracer newTracer;

void* operator new(size_t size, const char* file, long line)
{
	void * p = malloc(size);
	if (Tracer::_bReady)
	{
		newTracer.Add(p, file, line);
	}
	return p;
}

void operator delete(void* p, const char*, long)
{
	if (Tracer::_bReady)
	{
		newTracer.Remove(p);
	}
	free(p);
}

void* operator new(size_t size)
{
	void * p = malloc(size);
	if (Tracer::_bReady)
	{
		newTracer.Add(p, "?", 0); 
	}
	return p;
}

void operator delete(void* p)
{
	if (Tracer::_bReady)
	{
		newTracer.Remove(p);
	}
	free(p);
}

void* operator new[](size_t size, const char* file, long line)
{
	void * p = malloc(size);
	if (Tracer::_bReady)
	{
		newTracer.Add(p, file, line);
	}
	return p;
}
void operator delete[](void*p, const char*, long)
{
	if (Tracer::_bReady)
	{
		newTracer.Remove(p);
	}
	free(p);
}
void* operator new[](size_t size)
{
	void * p = malloc(size);
	if (Tracer::_bReady)
	{
		newTracer.Add(p, "?",0);
	}
	return p;
}
void operator delete[](void*p)
{
	if (Tracer::_bReady)
	{
		newTracer.Remove(p);
	}
	free(p);
}

#endif //NDEBUG