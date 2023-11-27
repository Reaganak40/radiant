#include "pch.h"
#include <Radiant/Utils/String.hpp>

struct rdt::String::Impl
{
	std::string m_string;

	~Impl()
	{
	}

	Impl()
	{
	}

};
// ==========================================

rdt::String::String()
	: m_impl(new String::Impl)
{
}

rdt::String::String(const String& other)
	: m_impl(new String::Impl)
{
	m_impl->m_string = other.m_impl->m_string;
}

rdt::String::String(const std::string& str)
	: m_impl(new String::Impl)
{
	m_impl->m_string = str;
}

rdt::String::String(const char* str)
	: m_impl(new String::Impl)
{
	m_impl->m_string = str;
}

rdt::String::~String()
{
	delete m_impl;
}

const char* rdt::String::c_str() const
{
	return m_impl->m_string.c_str();
}

void rdt::String::clear()
{
	m_impl->m_string.clear();
}

rdt::String& rdt::String::operator+=(const char c)
{
	m_impl->m_string += c;
	return *this;
}

rdt::String& rdt::String::operator+=(const std::string& str)
{
	m_impl->m_string += str;
	return *this;
}
