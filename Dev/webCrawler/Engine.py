class Engine:

    def __init__(self):
        self._currentTag = ''
        self._currentUrl = ''

    def addCurrentUrl(self, url):
        self._currentUrl = url

    def handle_starttag(self, tag, attrs):
        pass

    def handle_endtag(self, tag):
        self._currentTag = ''

    def handle_data(self, data):
        pass

    def analyze(self, maintype, subtype, data):
        pass
