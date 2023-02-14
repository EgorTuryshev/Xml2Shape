#ifndef TESTER_H
#define TESTER_H

#include <QDebug>
#include "xml_parser.h"

class tester
{
private:
    tester();
public:;
    static void testReadingHeaders(QString filePath);
    static void testReadingFeatureType(QString filePath);
    static void testReadingAttributes(QString filePath);
    static void testReadingDirectories();
    static void testReadingFeatures(QString filePath);
};

#endif // TESTER_H
