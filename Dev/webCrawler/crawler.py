from WebCrawler import WebCrawler
from IndexEngine import IndexEngine

if __name__ == '__main__':
    webCrawler = WebCrawler(10, engine=IndexEngine())
    webCrawler.addSeeds('http://www.gnulinuxmag.com', 'http://www.linux-pratique.com')
    webCrawler.start()
