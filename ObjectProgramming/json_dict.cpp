#include "json_object.h"
#include "json_dict.h"
#include "json_list.h"
#include "data/Integer.h"
#include "data/String.h"

void json_dict::put(json_object* key, json_object* val)
{
	v.push_back({ key, val });
}

int json_dict::find(json_object* key)
{
	std::string _key = key->to_string();

	for (int i = 0; i < v.size(); i++)
	{
		if (v[i].first->to_string() == _key)
			return i;
	}

	return -1;
}

json_dict::json_dict()
{
	v = std::vector<std::pair<json_object*, json_object*>>(0, { 0, 0 });
}

json_object* json_dict::parse(const char* str, int length)
{
	int _num = 0;
	int& _pos = json_object::_index;
	json_dict *tmp_dict = new json_dict();
	json_object *tmp_key = NULL, 
				*tmp_val = NULL;
	bool _isKey = true;
	
	while (_pos < length)
	{
		switch (str[_pos])
		{
			case '\'':
				++_pos;
				if (_isKey)
					tmp_key = String::parse(str, length, '\'');
				else
					tmp_val = String::parse(str, length, '\'');
				break;
			case ':':
				_isKey = false;
				break;
			case ',':
				_num = tmp_dict->find(tmp_key);
				if (_num != -1)
					tmp_dict->v[_num].second = tmp_val;
				else
					tmp_dict->put(tmp_key, tmp_val);
				_isKey = true;
				break;
			case '[':
				++_pos;
				tmp_val = json_list::parse(str, length);
				break;
			case '{':
				++_pos;
				tmp_val = json_dict::parse(str, length);
				break;
			case '}':
				_num = tmp_dict->find(tmp_key);
				if (_num != -1)
					tmp_dict->v[_num].second = tmp_val;
				else
					tmp_dict->put(tmp_key, tmp_val);
				return tmp_dict;
			case ' ':
				break;
			default:
				if (_isKey)
					tmp_key = Integer::parse(str, length);
				else
					tmp_val = Integer::parse(str, length);
				--_pos;
				break;
		}
		++_pos;
	}

	return tmp_dict;
}

json_object* json_dict::operator[](json_object* key) const
{
	if (key->type() == json_object::INT)
	{
		return operator[](atoi(key->to_string().c_str()));
	}
	else if (key->type() == json_object::STRING)
	{
		return operator[](key->to_string());
	}

	return NULL;
}

json_object* json_dict::operator[](const std::string& key) const
{
	std::string _key = '\'' + key + '\'';
	for (std::pair<json_object*, json_object*> _tmp : v)
	{
		if (_tmp.first->to_string() == _key)
			return _tmp.second;
	}
	return NULL;
}

json_object* json_dict::operator[](int key) const
{
	for (std::pair<json_object*, json_object*> _tmp : v)
	{
		int _key = atoi(_tmp.first->to_string().c_str());
		if (_key == key)
			return _tmp.second;
	}
	return NULL;
}

json_object::_type json_dict::type()
{
	return json_object::DICT;
}

std::string json_dict::to_string()
{
	if (v.size() == 0)
		return "{}";

	std::string _res = "{";
	for (int i = 0; i < v.size()-1; i++)
	{
		_res += v[i].first->to_string() + ": " + v[i].second->to_string();
		_res += ", ";
	}
	_res += v[v.size()-1].first->to_string() + ": " + v[v.size() - 1].second->to_string() + '}';
	return _res;
}