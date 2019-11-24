#include "json_object.h"
#include "json_dict.h"
#include "json_list.h"
#include "data/Integer.h"
#include "data/String.h"

int json_object::_index = 0;

json_object* json_object::parse(const std::string& str)
{
	return parse(str.c_str(), str.size());
}

json_object* json_object::parse(const char* str, int length)
{
	json_object* tmp = NULL;
	int start = 0;
	int end = 0;
	while (_index < length)
	{
		switch (str[_index])
		{
			case '{':
				++_index;
				tmp = json_dict::parse(str, length);
				break;
			case '[':
				++_index;
				tmp = json_list::parse(str, length);
				break;
			case '\'':
				++_index;
				tmp = String::parse(str, length, '\'');
				break;
			default:
				if (str[_index] > 47 && str[_index] < 57)
					tmp = Integer::parse(str, length);
				break;
		}
		++_index;
	}

	_index = 0;
	return tmp;
}