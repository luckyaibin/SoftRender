#ifndef _TRACER_H
#define _TRACER_H
#include "map"

#ifndef NDEBUG				//如果不是调试模式，忽略

void* operator new (size_t size, const char* file, long line);
void  operator delete (void* p, const char*, long);
void* operator new (size_t size);
void  operator delete (void* p);
void* operator new[] (size_t size, const char* file, long line);
void  operator delete[] (void* p, const char*, long);
void* operator new[] (size_t size);
void  operator delete[] (void* p);

class Tracer{
private:
	class Entry
	{
	public:
		Entry(const char*file = 0, long line = 0) :_file(file),_line(line){}
		const char* File(){ return _file; }
		long Line(){ return _line; }
	private:
		const char* _file;
		long _line;
	};
	class Lock 			//局部对象一定会释放，然后调用析构函数，防止了出现死锁
	{					//Lock 类是为了防止 void* operator new (size_t size); 函数被重复调用
	public:
		Lock(Tracer& tracer) :_tracer(tracer)
		{
			_tracer.lock();
		}
		~Lock()
		{
			_tracer.unlock();
		}
	private:
		Tracer& _tracer;
	};
	friend class Lock;
public:
	Tracer();
	~Tracer();
	void Add(void*p, const char*file, long line);
	void Remove(void* p);
	void Dump();
public:
	static bool _bReady;
private:
	std::map<void*, Entry> _mapEntry;
	int _lockCount;
	void lock(){ ++_lockCount; }
	void unlock(){ --_lockCount; }
	
};
extern Tracer newTracer;

#endif // !NDEBUG
#endif // !_TRACER_H

