#include "json_object.h"
#include "json_dict.h"
#include "json_list.h"
#include "data/Integer.h"
#include "data/String.h"

json_list::json_list()
{
	v = std::vector<json_object*>();
}

json_object* json_list::parse(const char* str, int length)
{
	int& _pos = json_object::_index;
	
	json_object* tmp_val = NULL;
	json_list* tmp_list = new json_list();

	while (_pos < length)
	{
		switch (str[_pos])
		{
		case '\'':
			++_pos;
			tmp_val = String::parse(str, length, '\'');
			break;
		case ',':
			if (tmp_val != NULL)
			{
				tmp_list->v.push_back(tmp_val);
				tmp_val = NULL;
			}
			break;
		case '[':
			++_pos;
			tmp_val = json_list::parse(str, length);
			break;
		case ']':
			if (tmp_val != NULL)
				tmp_list->v.push_back(tmp_val);
			return tmp_list;
		case '{':
			tmp_val = json_dict::parse(str, length);
			break;
		case ' ':
			break;
		default:
			tmp_val = Integer::parse(str, length);
			--_pos;
			break;
		}
		++_pos;
	}
	return NULL;
}

json_object* json_list::operator[](int key) const
{
	return v[key];
}

json_object::_type json_list::type()
{
	return json_object::LIST;
}

std::string json_list::to_string()
{
	std::string _res = "[";
	if (v.size() == 0)
		return "[]";

	for (int i = 0; i < v.size() - 1; i++)
	{
		_res += v[i]->to_string() + ", ";
	}
	
	_res += v[v.size()-1]->to_string() + "]";
	return _res;
}