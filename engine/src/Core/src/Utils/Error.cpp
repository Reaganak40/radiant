#include "pch.h"
#include <Core/Utils/Error.hpp>

class rdt_not_assigned_exception : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Radiant: Not Assigned Exception";
    }
} not_assigned_exception;

void rdt::core::ThrowRadiantError(RadiantErrorType type)
{
    switch (type) {
    case RadiantErrorType_NotAssigned:
        throw not_assigned_exception;
    default:
        throw;
    }
}
