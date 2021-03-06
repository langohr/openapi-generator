#include <iostream>
#include <sstream>
#include <cstdlib>
#include <map>
#include <list>
#include <cstring>
#include "Helpers.h"
#include "Object.h"

using namespace std;

using namespace Tizen::ArtikCloud;

void helper_func(JsonObject *object, const gchar* member_name, JsonNode *member_node,gpointer user_data)
{
	gchar* member_string;
	if (!JSON_NODE_HOLDS_VALUE(member_node)) {
		if (JSON_NODE_HOLDS_OBJECT(member_node)) {
			JsonNode* tempnode = json_node_alloc();
			json_node_init_object(tempnode,json_node_get_object(member_node));
			member_string = json_to_string(tempnode,false);
			json_node_free(tempnode);
		} else {
			JsonNode* tempnode = json_node_alloc();
			json_node_init_array(tempnode,json_node_get_array(member_node));
			member_string = json_to_string(tempnode,false);
			json_node_free(tempnode);
		}
	} else {
		member_string = (char*) json_node_get_string(member_node);
		if (member_string == NULL) {
			char * valueType = (char*) json_node_type_name(member_node);
			if(strcmp(valueType,"Boolean") == 0){
				gboolean member_boolean = json_node_get_boolean(member_node);
				if (member_boolean == 0) {
					member_string = new char[std::strlen("false") + 1];
					strcpy(member_string, "false");
				} else {
					member_string = new char[std::strlen("true") + 1];
					strcpy(member_string, "true");
				}
			}
			else if(strcmp(valueType,"Integer") == 0){
				long long int member_int =  json_node_get_int(member_node);
				ostringstream ss;
				ss << member_int;
				string temp_member  = ss.str();
				const char * temp_char = temp_member.c_str();
				member_string = new char[strlen(temp_char) + 1];
				strcpy(member_string, temp_char);
			}
			else{
				double member_double =  json_node_get_double(member_node);
				ostringstream ss;
				ss << member_double;
				string temp_member  = ss.str();
				const char * temp_char = temp_member.c_str();
				member_string = new char[strlen(temp_char) + 1];
				strcpy(member_string, temp_char);
			}
		}
	}
	map<string,string>* new_map = static_cast<map<string,string>*>(user_data);
	(*new_map)[string(member_name)] = string(member_string);
	delete[] member_string;
}

char * json_to_string(JsonNode *node, gboolean pretty)
{
	JsonGenerator *generator;
	char *retval;

	g_return_val_if_fail(node != NULL, NULL);

	generator = json_generator_new();
	json_generator_set_pretty(generator, pretty);
	json_generator_set_root(generator, node);

	retval = json_generator_to_data(generator, NULL);

	g_object_unref(generator);

	return retval;
}

JsonNode *
json_from_string(const char *str, GError **mygerror)
{
	JsonParser *parser;
	JsonNode *retval;

	g_return_val_if_fail(str != NULL, NULL);

	mygerror = NULL;
	parser = json_parser_new();
	if (!json_parser_load_from_data(parser, str, -1, mygerror))
	{
		g_object_unref(parser);
		if(str == NULL)
			return NULL;
		retval = json_node_alloc();
		retval = json_node_init_string (retval, str);
		return retval;
	}

	retval = json_node_copy(json_parser_get_root(parser));

	g_object_unref(parser);

	return retval;
}

JsonNode*
converttoJson(void* ptr, string type, string containerType)
{
	if (ptr == NULL)
		return NULL;
	if (strcmp(containerType.c_str(), "array") == 0) {
		if (!type.empty()) {
			JsonArray* json_array = json_array_new();
			JsonNode* node = json_node_alloc();
			JsonNode* node_temp;


			if (strcmp("bool", type.c_str()) == 0) {
				list<bool>* new_list = static_cast<std::list<bool>*> (ptr);
				for (list<bool>::iterator it = (*new_list).begin(); it != (*new_list).end(); it++) {
					bool b = *it;
					node_temp = converttoJson(&b, type, "");
					json_array_add_element(json_array, node_temp);
				}
			} else if (strcmp("std::string", type.c_str()) == 0) {
				list<string>* new_list = static_cast<std::list<string>*> (ptr);
				for (list<string>::iterator it = (*new_list).begin(); it != (*new_list).end(); it++) {
					string b = *it;
					node_temp = converttoJson(&b, type, "");
					json_array_add_element(json_array, node_temp);
				}
			} else if (strcmp("int", type.c_str()) == 0) {
				list<int>* new_list = static_cast<std::list<int>*> (ptr);
				for (list<int>::iterator it = (*new_list).begin(); it != (*new_list).end(); it++) {
					int b = *it;
					node_temp = converttoJson(&b, type, "");
					json_array_add_element(json_array, node_temp);
				}
			} else if (strcmp("long long", type.c_str()) == 0) {
				list<long long>* new_list = static_cast<std::list<long long>*> (ptr);
				for (list<long long>::iterator it = (*new_list).begin(); it != (*new_list).end(); it++) {
					long long b = *it;
					node_temp = converttoJson(&b, type, "");
					json_array_add_element(json_array, node_temp);
				}
			} else if (strcmp("float", type.c_str()) == 0) {
				list<float>* new_list = static_cast<std::list<float>*> (ptr);
				for (list<float>::iterator it = (*new_list).begin(); it != (*new_list).end(); it++) {
					float b = *it;
					node_temp = converttoJson(&b, type, "");
					json_array_add_element(json_array, node_temp);
				}
			} else if (strcmp("double", type.c_str()) == 0) {
				list<double>* new_list = static_cast<std::list<double>*> (ptr);
				for (list<double>::iterator it = (*new_list).begin(); it != (*new_list).end(); it++) {
					double b = *it;
					node_temp = converttoJson(&b, type, "");
					json_array_add_element(json_array, node_temp);
				}
			} else {
				cout << "This case not handled in Sami Helpers. Make a loop and iterate there." << endl;
				//Object* obj = static_cast<Object*> (ptr);
				//GError *mygerror = (GError*) malloc(sizeof (GError));
				//JsonNode *node = json_from_string(obj->toJson(), &mygerror);
				//return node;
			}

			json_node_init_array(node, json_array);
			json_array_unref(json_array);
			return node;
		}
		return NULL;
	} else if (strcmp("std::string", type.c_str()) == 0) {
		JsonNode* node = json_node_alloc();
		string* v = static_cast<string*> (ptr);
		//const_gchar* b = v;
		json_node_init(node, JSON_NODE_VALUE);
		json_node_set_string(node, v->c_str());
		return node;
	} else if (strcmp("int", type.c_str()) == 0) {
		JsonNode* node = json_node_alloc();
		int* v = static_cast<int*> (ptr);
		gint b = *v;
		json_node_init(node, JSON_NODE_VALUE);
		json_node_set_int(node, b);
		return node;
	} else if (strcmp("float", type.c_str()) == 0) {
		JsonNode* node = json_node_alloc();
		float* v = static_cast<float*> (ptr);
		gdouble b = (double) *v;
		json_node_init(node, JSON_NODE_VALUE);
		json_node_set_double(node, b);
		return node;
	} else if (strcmp("long long", type.c_str()) == 0) {
		JsonNode* node = json_node_alloc();
		long long* v = static_cast<long long*> (ptr);
		gint64 b = (long long) *v;
		json_node_init(node, JSON_NODE_VALUE);
		json_node_set_int(node, b);
		return node;
	} else if (strcmp("double", type.c_str()) == 0) {
		JsonNode* node = json_node_alloc();
		double* v = static_cast<double*> (ptr);
		gdouble b = *v;
		json_node_init(node, JSON_NODE_VALUE);
		json_node_set_double(node, b);
		return node;
	} else if (strcmp("bool", type.c_str()) == 0) {
		JsonNode* node = json_node_alloc();
		bool* v = static_cast<bool*> (ptr);
		gboolean b = *v;
		json_node_init(node, JSON_NODE_VALUE);
		json_node_set_boolean(node, b);
		return node;
	}
	else if (!isprimitive(type)) {
		//Object* obj = static_cast<Object*> (ptr);
		//GError *mygerror = (GError*) malloc(sizeof (GError));
		//JsonNode *node = json_from_string(obj->toJson(), &mygerror);
		//return node;
	}
	return NULL;
}

// DateTime Class to be seen
// void
// toISO8601(String str, DateTime* dt) {
//   int idx, start;
//   int year, month, day, hour, minute, second;

//   start = 0;
//   str.IndexOf("-", start, idx);
//   String yearString;
//   str.SubString(0, idx, yearString);
//   Integer::Parse(yearString, year);

//   start = idx+1;
//   str.IndexOf("-", start, idx);
//   String monthString;
//   str.SubString(start, idx - start, monthString);
//   Integer::Parse(monthString, month);

//   start = idx+1;
//   str.IndexOf("T", start, idx);
//   String dayString;
//   str.SubString(start, idx - start, dayString);
//   Integer::Parse(dayString, day);

//   start = idx+1;
//   str.IndexOf(":", start, idx);
//   if(idx > 0) {
//	 String hourString;
//	 str.SubString(start, idx - start, hourString);
//	 Integer::Parse(hourString, hour);
//   }

//   start = idx+1;
//   str.IndexOf(":", start, idx);
//   if(idx > 0) {
//	 String minuteString;
//	 str.SubString(start, idx - start, minuteString);
//	 Integer::Parse(minuteString, minute);
//   }

//   start = idx+1;
//   str.IndexOf("+", start, idx);
//   if(idx > 0) {
//	 String secondString;
//	 str.SubString(start, idx - start, secondString);
//	 Integer::Parse(secondString, second);
//   }

//   dt->SetValue(year, month, day, hour, minute, second);
// }

void
jsonToValue(void* target, JsonNode* node, string type, string innerType)
{
	if (target == NULL || node == NULL || !JSON_NODE_HOLDS_VALUE(node)) {
		return;
	} else if (strcmp("bool", type.c_str()) == 0) {
		bool* val = static_cast<bool*> (target);
		*val = json_node_get_boolean(node);
	} else if (strcmp("int", type.c_str()) == 0) {
		int* val = static_cast<int*> (target);
		*val = json_node_get_int(node);
	} else if (strcmp("float", type.c_str()) == 0) {
		float* val = static_cast<float*> (target);
		*val = (float)(json_node_get_double(node));
	} else if (strcmp("long long", type.c_str()) == 0) {
		long long* val = static_cast<long long*> (target);
		*val = (long long)(json_node_get_int(node));
	} else if (strcmp("double", type.c_str()) == 0) {
		double* val = static_cast<double*> (target);
		*val = json_node_get_double(node);
	} else if (strcmp("std::string", type.c_str()) == 0) {
		string* val = static_cast<string*> (target);
		const gchar *temp = json_node_get_string(node);
		if (node == NULL) {

		}
	*val = string(temp);
	}
	//Implement
	// else if(type.StartsWith("DateTime", 0)) {
	//   JsonString* json = static_cast< JsonString* >(ptr);
	//   String str;
	//   str.Append(json->GetPointer());

	//   DateTime* val = static_cast< DateTime* > (target);
	//   toISO8601(str, val);
	// }
	else if (!isprimitive(type)) {
		cout<<"SOMETHING IS WRONG"<<endl;
	}
	//Implement
	// else if(type.StartsWith("IList", 0)) {
	//   IList* obj = static_cast< IList* >(target);
	//   JsonArray* pJsonArray = static_cast< JsonArray* >(ptr);

	//   IEnumeratorT< IJsonValue* >* pEnum = pJsonArray->GetEnumeratorN();
	//   while (pEnum->MoveNext() == E_SUCCESS) {
	//	 IJsonValue* pJsonValue = NULL;
	//	 pEnum->GetCurrent(pJsonValue);
	//	 void* updatedTarget = NULL;
	//	 updatedTarget = create(innerType);

	//	 if(updatedTarget != NULL) {
	//	   jsonToValue(updatedTarget, pJsonValue, innerType, "");
	//	   obj->Add((Object*)updatedTarget);
	//	 }
	//   }
	//   delete pEnum;
	// }
}


string
stringify(void* ptr, string type)
{
	if(ptr == NULL){
		string retval = "";
		return retval;
	} else if (strcmp("std::string", type.c_str()) == 0) {
		string * str = static_cast<string*> (ptr);
		string retval = string(*str);
		return retval;
	} else if (strcmp("int", type.c_str()) == 0) {
		int* pInt = static_cast<int*> (ptr);
		stringstream ss;
		ss << *pInt;
		string retval = ss.str();
		return retval;
	} else if (strcmp("long long", type.c_str()) == 0) {
		long long* pLong = static_cast<long long*> (ptr);
		stringstream ss;
		ss << *pLong;
		string retval = ss.str();
		return retval;
	} else if (strcmp("bool", type.c_str()) == 0) {
		bool* pBool = static_cast<bool*> (ptr);
		string retval;
		if (*pBool == true) {
			retval = "true";
		} else {
			retval = "false";
		}
		return retval;
	} else if (strcmp("double", type.c_str()) == 0) {
		double* pDouble = static_cast<double*> (ptr);
		stringstream ss;
		ss << *pDouble;
		string retval = ss.str();
		return retval;
	} else if (strcmp("float", type.c_str()) == 0) {
		float* pFloat = static_cast<float*> (ptr);
		stringstream ss;
		ss << *pFloat;
		string retval = ss.str();
		return retval;
	}
	return "";
}

bool isprimitive(string type){
	if(strcmp("std::string", type.c_str()) == 0||
		strcmp("int", type.c_str()) == 0||
		strcmp("long long", type.c_str()) == 0||
		strcmp("double", type.c_str()) == 0||
		strcmp("float", type.c_str()) == 0||
		strcmp("bool", type.c_str()) == 0||
		strcmp("std::map", type.c_str()) == 0||
		strcmp("std::list", type.c_str()) == 0) {
		return true;
		}
		return false;
}
