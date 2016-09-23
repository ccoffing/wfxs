#include "XSProperties.h"
#include "XSDataIO.h"


void XSProperties::SetDefaults()
{
    if (m_fabric.length() == 0)
        m_fabric = "Aida cloth";
    if (m_horiClothCount == 0)
        m_horiClothCount = 14;
    if (m_vertClothCount == 0)
        m_vertClothCount = m_horiClothCount;
}

XSProperties::XSProperties()
    : m_filename()
    , m_title()
    , m_author()
    , m_fabric()
    , m_instructions()
    , m_horiClothCount(0)
    , m_vertClothCount(0)
{
    SetDefaults();
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

void XSProperties::Serialize(std::ostream& file) const
{
    WriteCStr_exc(file, m_title);
    WriteCStr_exc(file, m_author);
    WriteCStr_exc(file, m_fabric);
    WriteCStr_exc(file, m_instructions);
    WriteLE32_exc(file, m_horiClothCount);
    WriteLE32_exc(file, m_vertClothCount);
}

void XSProperties::Unserialize(std::istream& file)
{
    ReadCStr_exc(file, m_title);
    ReadCStr_exc(file, m_author);
    ReadCStr_exc(file, m_fabric);
    ReadCStr_exc(file, m_instructions);
    ReadLE32_exc(file, m_horiClothCount);
    ReadLE32_exc(file, m_vertClothCount);
}
