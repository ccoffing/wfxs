#include "Logger.h"
#include "XSProperties.h"
#include "XSDataIO.h"

#define LOG_NAME "xs.Properties"

XSProperties::XSProperties()
    : m_filename()
    , m_title()
    , m_author()
    , m_fabric()
    , m_instructions()
    , m_horiClothCount(0)
    , m_vertClothCount(0)
{
    setDefaults();
}

XSProperties::XSProperties(XSProperties const& ref)
    : m_filename(ref.m_filename)
    , m_title(ref.m_title)
    , m_author(ref.m_author)
    , m_fabric(ref.m_fabric)
    , m_instructions(ref.m_instructions)
    , m_horiClothCount(ref.m_horiClothCount)
    , m_vertClothCount(ref.m_vertClothCount)
{
}

XSProperties& XSProperties::operator=(XSProperties const& rhs)
{
    if (this != &rhs) {
        m_filename = rhs.m_filename;
        m_title = rhs.m_title;
        m_author = rhs.m_author;
        m_fabric = rhs.m_fabric;
        m_instructions = rhs.m_instructions;
        m_horiClothCount = rhs.m_horiClothCount;
        m_vertClothCount = rhs.m_vertClothCount;
    }
    return *this;
}

XSProperties::~XSProperties()
{
}

void XSProperties::setDefaults()
{
    if (m_fabric.length() == 0)
        m_fabric = "Aida cloth";
    if (m_horiClothCount == 0)
        m_horiClothCount = 14;
    if (m_vertClothCount == 0)
        m_vertClothCount = m_horiClothCount;
}

void XSProperties::serialize(std::ostream& file) const
{
    WriteCStr_exc(file, m_title);
    WriteCStr_exc(file, m_author);
    WriteCStr_exc(file, m_fabric);
    WriteCStr_exc(file, m_instructions);
    WriteBE32_exc(file, m_horiClothCount);
    WriteBE32_exc(file, m_vertClothCount);
}

void XSProperties::unserialize(std::istream& file)
{
    ReadCStr_exc(file, m_title);
    Log::debug(LOG_NAME, "Title is '%s'", m_title.c_str());
    ReadCStr_exc(file, m_author);
    Log::debug(LOG_NAME, "Author is '%s'", m_author.c_str());
    ReadCStr_exc(file, m_fabric);
    Log::debug(LOG_NAME, "Fabric is '%s'", m_fabric.c_str());
    ReadCStr_exc(file, m_instructions);
    Log::debug(LOG_NAME, "Instructions is '%s'", m_instructions.c_str());
    ReadBE32_exc(file, m_horiClothCount);
    ReadBE32_exc(file, m_vertClothCount);
    Log::debug(LOG_NAME, "Cloth count is %ux%u", m_horiClothCount, m_vertClothCount);
}
