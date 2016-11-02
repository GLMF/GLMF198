from html.parser import HTMLParser
from urllib.request import urlopen
from urllib import parse

class WebCrawler(HTMLParser):

    def __init__(self, maxPages, engine=None, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.__crawlFrontier = []
        self.__seeds = []
        self.__maxPages = maxPages
        self.__currentUrl = None
        self.__visited = []
        self.__engine = engine

    def addSeeds(self, *args):
        for url in args:
            self.__seeds.append(url)

    def __initCrawler(self):
        self.__crawlFrontier = self.__seeds.copy()

    def __addCrawlFrontier(self, url):
        url = parse.urljoin(self.__currentUrl, url)
        if url not in self.__visited:
            self.__crawlFrontier.append(url)

    def __currentVisited(self):
        return self.__currentUrl in self.__visited

    def __addCurrentVisited(self):
        self.__visited.append(self.__currentUrl)

    def handle_starttag(self, tag, attrs):
        if tag == 'a':
            for attr, value in attrs:
                if attr == 'href':
                    self.__addCrawlFrontier(value)
                    break
        if self.__engine is not None:
            self.__engine.handle_starttag(tag, attrs)

    def handle_endtag(self, tag):
        if self.__engine is not None:
            self.__engine.handle_endtag(tag)

    def handle_data(self, data):
        if self.__engine is not None:
            self.__engine.handle_data(data)

    def start(self):
        pages = 0
        self.__initCrawler()
        while pages < self.__maxPages:
            try:
                if self.__crawlFrontier == []:
                    self.__stop()
                self.__currentUrl = self.__crawlFrontier.pop(0)
                if not self.__currentVisited():
                    pages += 1
                    self.__addCurrentVisited()
                    print('Step {} : visiting : {}'.format(pages, self.__currentUrl), end='')
                    self.__engine.addCurrentUrl(self.__currentUrl)

                    # Read the page
                    response = urlopen(self.__currentUrl)
                    maintype = response.info().get_content_maintype()
                    subtype = response.info().get_content_subtype()
                    if maintype == 'text' and subtype == 'html':
                        htmlBytes = response.read()
                        htmlString = htmlBytes.decode("utf-8")
                        self.feed(htmlString)
                    if self.__engine is not None:
                        self.__engine.analyze(maintype, subtype, htmlString)
                    print(' [OK]')
            except Exception as e:
                print(' [FAILED]', e)
        self.__stop()

    def __stop(self, callback=None):
        print('*** End ***')
        exit(0)
