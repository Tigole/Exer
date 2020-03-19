#ifndef _XML_FILE_LOADER_HPP
#define _XML_FILE_LOADER_HPP 1

#include <tinyxml.h>

#include <map>
#include <string>
#include <functional>
#include <stack>

class XML_Element
{
public:
	XML_Element(const TiXmlElement& target);
	bool mt_Get_Attribute(const std::string& attribute_name, std::string& attribute_value) const;
	bool mt_Get_Attribute(const std::string& attribute_name, int& attribute_value) const;
	bool mt_Get_Attribute(const std::string& attribute_name, unsigned int& attribute_value) const;
	bool mt_Get_Attribute(const std::string& attribute_name, float& attribute_value) const;
	bool mt_Get_Attribute(const std::string& attribute_name, bool& attribute_value) const;
	template<typename T>
	bool mt_Get_Attribute(const std::string& attribute_name, T& t) const
	{
		int i;
		bool l_b_Ret;

		l_b_Ret = mt_Get_Attribute(attribute_name, i);
		if (l_b_Ret == true)
		{
			t = static_cast<T>(i);
		}

		return l_b_Ret;
	}
	template<typename T>
	bool mt_Get_Attribute(const std::string& attribute_name, T& attribute_value, T(*pfn_StringToEnum)(const std::string&)) const
	{
		bool l_b_ret;
		std::string l_str;

		l_b_ret = mt_Get_Attribute(attribute_name, l_str);

		if (l_b_ret == true)
		{
			attribute_value = pfn_StringToEnum(l_str);
		}


		return l_b_ret;
	}
	bool mt_Get_Value(std::string& element_value) const;

	const char* mt_Get_Text(void) const;

private:
	const TiXmlElement& m_target;
};

using XML_Callback = std::function<bool(const XML_Element&)>;
using XML_CallbackContainer = std::map<std::string, XML_Callback>;
struct XML_FileHandlerData
{
	XML_FileHandlerData() : m_element_count(0), m_on_entry_callbacks(), m_on_exit_callbacks(){}
	size_t m_element_count;
	XML_CallbackContainer m_on_entry_callbacks;
	XML_CallbackContainer m_on_exit_callbacks;
};
using XML_FileHandler = std::map<std::string, XML_FileHandlerData>;

class XMLFileLoader
{
public:
	XMLFileLoader();

	bool mt_Set_File(const std::string& file_path);
	template<class C>
	bool mt_Add_On_Entry_Callback(const std::string xml_path, bool(C::*callback)(const XML_Element&), C* object)
	{
		bool l_b_ret;
		if (m_current_file_it != m_files.end())
		{
			l_b_ret = mt_Add_On_Entry_Callback(m_current_file_it->first, xml_path, callback, object);
		}
		else
		{
			l_b_ret = false;
		}
		return l_b_ret;
	}
	template<class C>
	bool mt_Add_On_Entry_Callback(const std::string& file_path, const std::string xml_path, bool(C::*callback)(const XML_Element&), C* object)
	{
		bool l_b_ret;
		XML_FileHandler::iterator l_file_it;

		l_b_ret = mt_Add_File(file_path);

		l_file_it = m_files.find(file_path);
		if (l_file_it != m_files.end())
		{
			l_b_ret = mt_Add_Callback(l_file_it->second.m_on_entry_callbacks, xml_path, callback, object);
		}

		return l_b_ret;
	}
	template<class C>
	bool mt_Add_On_Exit_Callback(const std::string xml_path, bool(C::*callback)(const XML_Element&), C* object)
	{
		bool l_b_ret;
		if (m_current_file_it != m_files.end())
		{
			l_b_ret = mt_Add_On_Exit_Callback(m_current_file_it->first, xml_path, callback, object);
		}
		else
		{
			l_b_ret = false;
		}
		return l_b_ret;
	}
	template<class C>
	bool mt_Add_On_Exit_Callback(const std::string& file_path, const std::string xml_path, bool(C::*callback)(const XML_Element&), C* object)
	{
		XML_FileHandler::iterator l_file_it;
		bool l_b_ret;

		l_b_ret = mt_Add_File(file_path);

		l_file_it = m_files.find(file_path);
		if (l_file_it != m_files.end())
		{
			l_b_ret = mt_Add_Callback(l_file_it->second.m_on_exit_callbacks, xml_path, callback, object);
		}

		return l_b_ret;
	}
	void mt_Work(void);

protected:
	template<class C>
	bool mt_Add_Callback(XML_CallbackContainer& callback_container, const std::string& xml_path, bool(C::*callback)(const XML_Element&), C* object)
	{
		return callback_container.emplace(xml_path, std::bind(callback, object, std::placeholders::_1)).second;
	}
	bool mt_Add_File(const std::string& file_path);
	struct LoadingStructure
	{
		LoadingStructure():m_error(), m_element_count(0){}
		std::string m_error;
		size_t m_element_count;
	};
	bool mt_Explore_Document(TiXmlElement& root, LoadingStructure& loading_struct, XML_CallbackContainer& on_entry_callbacks, XML_CallbackContainer& on_exit_callbacks);
	bool mt_Recursive_Exploration(const TiXmlElement* current_element, LoadingStructure& loading_struct, XML_CallbackContainer& on_entry_callbacks, XML_CallbackContainer& on_exit_callbacks);
	bool mt_Manage_Callback(const TiXmlElement& element, const std::string& path, XML_CallbackContainer& callbacks);
	std::string mt_Get_Path(const TiXmlNode* element);

	XML_FileHandler m_files;
	XML_FileHandler::iterator m_current_file_it;
	LoadingStructure m_loading_struct;
};

#endif // _XML_FILE_LOADER_HPP
