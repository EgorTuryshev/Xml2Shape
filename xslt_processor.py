from lxml import etree
import sys

def main(xml_file, xslt_file):
    xml_tree = etree.parse(xml_file)
    xslt_tree = etree.parse(xslt_file)
    transform = etree.XSLT(xslt_tree)
    result = transform(xml_tree)
    #f = open('tmpxml.xml', 'w')
    #f.write(str(result))
    #f.close()
    print(str(result))

if __name__ == '__main__':
    if len(sys.argv) != 3:
        sys.exit(1)
    main(sys.argv[1], sys.argv[2])
