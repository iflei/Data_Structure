#pragma once

template <typename K>
struct __HashFunc1
{
	size_t BKDRHash(const char *str)
	{
		register size_t hash = 0;
		while (*str)
		{
			hash = hash * 131 + *str++;   // 也可以乘以31、131
		}
		return hash;
	}

	size_t operator()(const K& key)
	{
		return BKDRHash(key.c_str());
	}
};

template <typename K>
struct __HashFunc2
{
	size_t SDBMHash(const char *str)
	{
		register size_t hash = 0;
		while (size_t ch = (size_t)*str++)
		{
			hash = 65599 * hash + ch;
			//hash = (size_t)ch + (hash << 6) + (hash << 16) - hash;  
		}
		return hash;
	}

	size_t operator()(const K& key)
	{
		return SDBMHash(key.c_str());
	}
};

template <typename K>
struct __HashFunc3
{
	size_t JSHash(const char *str)
	{
		if (!*str)        // 这是由本人添加，以保证空字符串返回哈希值0  
			return 0;
		register size_t hash = 1315423911;
		while (size_t ch = (size_t)*str++)
		{
			hash ^= ((hash << 5) + ch + (hash >> 2));
		}
		return hash;
	}

	size_t operator()(const K& key)
	{
		return JSHash(key.c_str());
	}
};

template <typename K>
struct __HashFunc4
{
	size_t APHash(const char *str)
	{
		register size_t hash = 0;
		size_t ch;
		for (long i = 0; ch = (size_t)*str++; i++)
		{
			if ((i & 1) == 0)
				hash ^= ((hash << 7) ^ ch ^ (hash >> 3));
			else
				hash ^= (~((hash << 11) ^ ch ^ (hash >> 5)));
		}
		return hash;
	}

	size_t operator()(const K& key)
	{
		return APHash(key.c_str());
	}
};

template <typename K>
struct __HashFunc5
{
	size_t RSHash(const char *str)
	{
		register size_t hash = 0;
		size_t magic = 63689;
		while (size_t ch = (size_t)*str++)
		{
			hash = hash * magic + ch;
			magic *= 378551;
		}
		return hash;
	}

	size_t operator()(const K& key)
	{
		return RSHash(key.c_str());
	}
};