from html.parser import HTMLParser
from urllib.request import urlopen
from urllib import parse

class TestHtmlParser(HTMLParser):

    def __init__(self, header_n, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.__currentTag = ''
        self.__search = 'h' + str(header_n)

    def handle_starttag(self, tag, attrs):
        if tag == self.__search:
            self.__currentTag = self.__search

    def handle_endtag(self, tag):
        self.__currentTag = ''

    def handle_data(self, data):
        if self.__currentTag == self.__search:
            print('{} => {}'.format(self.__search, data))

  
if __name__ == '__main__':      
    parser = TestHtmlParser(1)
    url = 'http://www.gnulinuxmag.com'
    print('Ouverture de', url)
    response = urlopen(url)
    maintype = response.info().get_content_maintype()
    subtype = response.info().get_content_subtype()
    if maintype == 'text' and subtype == 'html':
        htmlBytes = response.read()
        htmlString = htmlBytes.decode("utf-8")
        parser.feed(htmlString)
    print('*** End ***')
