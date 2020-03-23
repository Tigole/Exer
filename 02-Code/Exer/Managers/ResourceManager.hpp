#ifndef _RESOURCE_MANAGER_HPP
#define _RESOURCE_MANAGER_HPP 1

#include "../_Misc.hpp"

#include <memory>
#include <unordered_map>
#include <string>
#include <sstream>
#include "FilePacker/FilePacker.hpp"

template<typename ResourceType>
struct Resource
{
    Resource(): m_Resource(nullptr), m_Resource_Id(nullptr){}
    Resource(ResourceType* r, const std::string* id): m_Resource(r), m_Resource_Id(id){}
    Resource(const Resource& r): m_Resource(r.m_Resource), m_Resource_Id(r.m_Resource_Id){}
    Resource& operator=(const Resource& r)
    {
        m_Resource = r.m_Resource;
        m_Resource_Id = r.m_Resource_Id;
        return *this;
    }
    ResourceType* operator->(){return m_Resource;}
    ResourceType& operator*(){return *m_Resource;}
    const ResourceType* operator->() const {return m_Resource;}
    const ResourceType& operator*() const {return *m_Resource;}
    bool operator==(nullptr_t)
    {
        return m_Resource == nullptr;
    }
    bool operator!=(nullptr_t)
    {
        return m_Resource != nullptr;
    }
    Resource& operator=(nullptr_t)
    {
        m_Resource = nullptr;
        m_Resource_Id = nullptr;
    }
    ResourceType* m_Resource;
    const std::string* m_Resource_Id;
};

template<typename ResourceType>
struct ResourceData
{
    ResourceData(const std::string& reference_file) : m_Counter(0), m_Reference_File(reference_file), m_Resource(nullptr) {}
    int m_Counter;
    std::string m_Reference_File;
    std::unique_ptr<ResourceType> m_Resource;
};

template<typename ResourceType>
using Container = std::unordered_map<std::string, std::unique_ptr<ResourceData<ResourceType>>>;


template<typename ResourceType>
class ResourceManager
{
public:
    ResourceManager(const std::string& manager_name) : m_Manager_Name(manager_name + " Manager"){}
    virtual ~ResourceManager(){}

    virtual bool mt_Load(const std::string& file_desc)
    {
        return true;
    }

    void mt_Load_CFG(const jaja::fp::File& file)
    {
        std::stringstream l_Stream;
        std::string l_Line;

        l_Stream.str(file.mt_To_String());

        while(fn_Get_Line(l_Stream, l_Line))
        {
            std::string::size_type l_Separator;
            std::string l_Resource_Id;
            std::string l_Resource_Path;

            l_Separator = l_Line.find(':');

            l_Resource_Id = l_Line.substr(0, l_Separator);
            l_Resource_Path = l_Line.substr(l_Separator + 1, std::string::npos);

            mt_Add_Resource(l_Resource_Id, new ResourceData<ResourceType>(l_Resource_Path));
        }
    }

    Resource<ResourceType> mt_Get_Resource(const std::string& resource_id)
    {
        auto l_it = m_Resources.find(resource_id);
        if (l_it == m_Resources.end())
            throw std::runtime_error("[" + m_Manager_Name + "] Unknown resource: " + resource_id);

        ResourceData<ResourceType>& l_Resource_Data = *l_it->second;

        if (l_Resource_Data.m_Counter == 0)
        {
            if (mt_Load_Resource(l_Resource_Data.m_Reference_File, l_Resource_Data.m_Resource) == false)
                throw std::runtime_error("[" + m_Manager_Name + "] Fail loading: " + resource_id);
        }
        l_Resource_Data.m_Counter++;

        return Resource<ResourceType>(l_Resource_Data.m_Resource.get(), &l_it->first);
    }

    void mt_Release(const Resource<ResourceType>& resource)
    {
        if (resource.m_Resource_Id != nullptr)
        {
            auto l_it = m_Resources.find(*resource.m_Resource_Id);
            if (l_it == m_Resources.end())
                throw std::runtime_error("[" + m_Manager_Name + "] Unknown resource: " + *resource.m_Resource_Id);

            ResourceData<ResourceType>& l_Resource_Data = *l_it->second;

            l_Resource_Data.m_Counter--;

            if (l_Resource_Data.m_Counter == 0)
            {
                mt_Unload_Resource(l_Resource_Data.m_Resource.get());
                l_Resource_Data.m_Resource.reset(nullptr);
            }
        }
    }

protected:
    virtual bool mt_Load_Resource(const std::string& reference_file, std::unique_ptr<ResourceType>& resource) = 0;
    virtual void mt_Unload_Resource(ResourceType* resource) = 0;
    void mt_Add_Resource(const std::string& id, ResourceData<ResourceType>* resource_data)
    {
        m_Resources.emplace(id, std::unique_ptr<ResourceData<ResourceType>>(resource_data));
    }
    void mt_Auto_Load(const std::string& path)
    {
        std::vector<std::pair<std::string, std::string>> l_Files;
        std::string l_Path(path);

        if (l_Path.back() != '/' && l_Path.back() != '\\')
        {
            l_Path.push_back('/');
        }

        l_Files = fn_Get_Files(l_Path);

        for (std::size_t ii = 0; ii < l_Files.size(); ii++)
        {
            mt_Add_Resource(l_Files[ii].first, new ResourceData<ResourceType>(l_Path + l_Files[ii].first + "." + l_Files[ii].second));
        }
    }

private:
    std::string m_Manager_Name;
    Container<ResourceType> m_Resources;
};


#endif // _RESOURCE_MANAGER_HPP
