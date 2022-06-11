#pragma once

/*
	My solution makes use of the singleton design pattern for objects that I do not
	want copied or moved.
*/

class Singleton
{
protected:

	Singleton() = default;
	Singleton(const Singleton&) = delete;
	Singleton& operator= (const Singleton&) = delete;
};

