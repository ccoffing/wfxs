#ifndef XS_FILE_NATIVE_H
#define XS_FILE_NATIVE_H

#include <iostream>

class XSModel;

/**
 */
void StreamOutNative(std::ostream& stream, const XSModel* doc);

/**
 */
void StreamInNative(std::istream& stream, XSModel* doc);
#endif
