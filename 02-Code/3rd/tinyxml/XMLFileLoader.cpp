#include "XMLFileLoader.hpp"
#include <unordered_map>

XML_Element::XML_Element(const TiXmlElement& target)
 :	m_target(target)
{}

bool XML_Element::mt_Get_Attribute(const std::string& attribute_name, std::string& attribute_value) const
{
	return m_target.QueryStringAttribute(attribute_name.c_str(), &attribute_value) == TIXML_SUCCESS;
}

bool XML_Element::mt_Get_Attribute(const std::string& attribute_name, int& attribute_value) const
{
	return m_target.QueryIntAttribute(attribute_name.c_str(), &attribute_value) == TIXML_SUCCESS;
}

bool XML_Element::mt_Get_Attribute(const std::string& attribute_name, unsigned int& attribute_value) const
{
	return m_target.QueryUnsignedAttribute(attribute_name.c_str(), &attribute_value) == TIXML_SUCCESS;
}

bool XML_Element::mt_Get_Attribute(const std::string& attribute_name, float& attribute_value) const
{
	return m_target.QueryFloatAttribute(attribute_name.c_str(), &attribute_value) == TIXML_SUCCESS;
}

bool XML_Element::mt_Get_Attribute(const std::string& attribute_name, bool& attribute_value) const
{
	std::string l_attribute_value;
	bool l_b_ret;
	std::unordered_map<std::string, bool> l_map;
	std::unordered_map<std::string, bool>::const_iterator l_it;

	l_map.emplace(std::make_pair("true", true));
	l_map.emplace(std::make_pair("false", false));
	l_map.emplace(std::make_pair("TRUE", true));
	l_map.emplace(std::make_pair("FALSE", false));
	l_map.emplace(std::make_pair("yes", true));
	l_map.emplace(std::make_pair("no", false));
	l_map.emplace(std::make_pair("YES", true));
	l_map.emplace(std::make_pair("NO", false));
	l_map.emplace(std::make_pair("1", true));
	l_map.emplace(std::make_pair("0", false));

	l_b_ret = mt_Get_Attribute(attribute_name, l_attribute_value);
	if (l_b_ret == true)
	{
		l_it = l_map.find(l_attribute_value);
		if (l_it != l_map.end())
		{
			l_b_ret = true;
			attribute_value = l_it->second;
		}
		else
		{
			l_b_ret = false;
		}
	}

	return m_target.QueryBoolAttribute(attribute_name.c_str(), &attribute_value) == TIXML_SUCCESS;
}

bool XML_Element::mt_Get_Value(std::string& element_value) const
{
	element_value = m_target.ValueStr();
	return true;
}

const char* XML_Element::mt_Get_Text(void) const
{
    return m_target.GetText();
}

XMLFileLoader::XMLFileLoader()
 :	m_files(),
	m_current_file_it(m_files.end()),
	m_loading_struct()
{}

bool XMLFileLoader::mt_Set_File(const std::string& file_path)
{
	LoadingStructure l_loading_struct;
	TiXmlDocument l_document;
	TiXmlElement* l_root;
	XML_FileHandlerData l_xml_file_handler_data;
	bool l_b_ret;

	l_b_ret = l_document.LoadFile(file_path);
	if (l_b_ret == true)
	{
		l_root = l_document.RootElement();
		l_b_ret = (l_root != nullptr);
		if (l_b_ret == true)
		{
			m_current_file_it = m_files.find(file_path);
			if (m_current_file_it == m_files.end())
			{
				l_b_ret = mt_Explore_Document(*l_root, l_loading_struct, l_xml_file_handler_data.m_on_entry_callbacks, l_xml_file_handler_data.m_on_exit_callbacks);
				if (l_b_ret == true)
				{
					l_xml_file_handler_data.m_element_count = l_loading_struct.m_element_count;
					m_current_file_it = m_files.emplace(file_path, l_xml_file_handler_data).first;
				}
			}
		}
	}

	return l_b_ret;
}

bool XMLFileLoader::mt_Add_File(const std::string& file_path)
{
	XML_FileHandler::iterator l_file_it;
	bool l_b_ret;

	l_file_it = m_files.find(file_path);

	if (l_file_it == m_files.end())
	{
		l_file_it = m_current_file_it;
		l_b_ret = mt_Set_File(file_path);
		m_current_file_it = l_file_it;
	}
	else
	{
		l_b_ret = true;
	}

	return l_b_ret;
}

void XMLFileLoader::mt_Work(void)
{
	TiXmlDocument l_document;

	if (m_current_file_it != m_files.end())
	{
		if (l_document.LoadFile(m_current_file_it->first))
		{
			mt_Explore_Document(*l_document.RootElement(), m_loading_struct, m_current_file_it->second.m_on_entry_callbacks, m_current_file_it->second.m_on_exit_callbacks);
		}
	}
}

bool XMLFileLoader::mt_Manage_Callback(const TiXmlElement& element, const std::string& path, XML_CallbackContainer& callbacks)
{
	bool l_b_ret(true);
	XML_CallbackContainer::const_iterator l_it(callbacks.find(path));

	if (l_it != callbacks.end())
	{
		l_b_ret = (l_it->second)(XML_Element(element));
	}

	return l_b_ret;
}

std::string XMLFileLoader::mt_Get_Path(const TiXmlNode* element)
{
	std::string l_ret;
	std::stack<const TiXmlNode*> l_nodes;

	while (element != nullptr)
	{
		l_nodes.push(element);
		element = element->Parent();
	}

	l_nodes.pop();/** Remove document : we don't care **/

	while (l_nodes.size())
	{
		l_ret += "/" + l_nodes.top()->ValueStr();
		l_nodes.pop();
	}

	return l_ret;
}

bool XMLFileLoader::mt_Recursive_Exploration(const TiXmlElement* current_element, LoadingStructure& loading_struct, XML_CallbackContainer& on_entry_callbacks, XML_CallbackContainer& on_exit_callbacks)
{
	bool l_b_ret;
	const TiXmlElement* l_previous_element(current_element);
	std::string l_path;

	l_path = mt_Get_Path(current_element);
	l_b_ret = mt_Manage_Callback(*current_element, l_path, on_entry_callbacks);
	if (l_b_ret == true)
	{
		current_element = current_element->FirstChildElement();
		if (current_element != nullptr)
		{
			l_b_ret = mt_Recursive_Exploration(current_element, loading_struct, on_entry_callbacks, on_exit_callbacks);
		}

		l_path = mt_Get_Path(l_previous_element);
		l_b_ret = mt_Manage_Callback(*l_previous_element, l_path, on_exit_callbacks);
		if (l_b_ret == true)
		{
			current_element = l_previous_element->NextSiblingElement();
			if (current_element != nullptr)
			{
				l_b_ret = mt_Recursive_Exploration(current_element, loading_struct, on_entry_callbacks, on_exit_callbacks);
			}
		}
	}
	loading_struct.m_element_count++;

	return l_b_ret;
}

bool XMLFileLoader::mt_Explore_Document(TiXmlElement& root, LoadingStructure& loading_struct, XML_CallbackContainer& on_entry_callbacks, XML_CallbackContainer& on_exit_callbacks)
{
	bool l_ret(true);

	l_ret = mt_Recursive_Exploration(&root, loading_struct, on_entry_callbacks, on_exit_callbacks);

	return l_ret;
}
