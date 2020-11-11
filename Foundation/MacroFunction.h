#pragma once

#define NON_COPYABLE(name)                    \
name(const name&) = delete;                    \
name(const name&&) = delete;                \
name& operator=(const name&) = delete;        \
name& operator=(const name&&) = delete;

#define IN
#define OUT

