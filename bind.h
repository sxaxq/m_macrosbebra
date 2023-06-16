#pragma once
#include <vector>
#include <string>
#include <iostream>
using namespace std;

class bind_t
{
private:
	size_t id;
	int key_code;
	std::vector<std::string> text;
	int pause;
public:
	class bind_t() = default;
	class bind_t(size_t id_) : id(id_), pause(1000) {}

	int get_pause()
	{
		return pause;
	}

	size_t get_id()
	{
		return id;
	}

	char read_key()
	{
		return char(key_code);
	}

	void set_key(int vk)
	{
		key_code = vk;
	}

	std::vector<std::string> get_text()
	{
		return text;
	}

	void push_text(std::vector<std::string>& v)
	{
		text = v;
	}

	void set_pause(int mseconds)
	{
		pause = mseconds;
	}

	void show_info()
	{
		cout << "=========================================" << endl;
		cout << "KEY: " << key_code << endl;
		cout << "String count: " << text.size() << endl;
		cout << "Pause: " << pause << endl;
		cout << "=========================================" << endl;
	}
};

bool isContains(std::vector<bind_t>& binds, int key_code)
{
	for (size_t i = 0; i < binds.size(); i++)
	{
		if (binds[i].read_key() == key_code)
		{
			return true;
		}
	}
	return false;
}

int getPause(std::vector<bind_t>& binds, int key_code)
{
	for (size_t i = 0; i < binds.size(); i++)
	{
		if (binds[i].read_key() == key_code)
		{
			return binds[i].get_pause();
		}
	}

	return 0;
}

std::vector<std::string> getBind(std::vector<bind_t>& binds, int key_code)
{
	for (size_t i = 0; i < binds.size(); i++)
	{
		if (binds[i].read_key() == key_code)
		{
			return binds[i].get_text();
		}
	}
	std::vector<std::string> v;
	return v;
}