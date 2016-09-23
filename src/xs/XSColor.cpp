#include "XSColor.h"

std::ostream& operator<<(std::ostream& out, XSColor const& c)
{
    out.put(c.red);
    out.put(c.green);
    out.put(c.blue);
    out.put(c.alpha);
    return out;
}

bool XSColor::operator==(XSColor const& rhs) const
{
    return red == rhs.red && green == rhs.green && blue == rhs.blue && alpha == rhs.alpha;
}
