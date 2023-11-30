#include "pch.h"
#include <Core/Utils/Error.hpp>

class rdt_not_assigned_exception : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Radiant: Not Assigned Exception";
    }
} not_assigned_exception;

class rdt_component_overflow_exception : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Radiant: Component Overflow Exception";
    }
} component_overflow_exception;

void rdt::core::ThrowRadiantError(RadiantErrorType type)
{
    switch (type) {
    case RadiantErrorType_NotAssigned:
        throw not_assigned_exception;
    case RadiantErrorType_ComponentOverflow:
        throw component_overflow_exception;
    default:
        throw;
    }
}
