#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

class PatchException
{
public:
	PatchException(const char* str) : m_str(str) {}
	const char* GetDescription() { return m_str; }
private:
	const char* m_str;
};

#endif // __EXCEPTIONS_H__
