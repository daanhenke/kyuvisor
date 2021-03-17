#pragma once

#include "efi/core.hh"

#define PROTOCOL_METHOD(typename, arguments) typedef kyu::efi::status_t (* typename) arguments;