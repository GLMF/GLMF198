from Engine import Engine
import redis

class IndexEngine(Engine):

    IGNORE = ['', 'de', 'du', 'des', 'dans', 'le', 'la', 'les', 'a', 'à', 'au', 'aux',
              'sur', 'et', 'je', 'tu', 'il', 'elle', 'on', 'nous', 'vous', 'ils', 'elles',
              'par', 'si', 'que', 'qui', 'quoi', 'ou', 'où', 'chez', '|', '!', 'pour']

    def __init__(self, host='localhost', port=6379):
        super().__init__()
        self.__redis = redis.StrictRedis(host=host, port=port)
        self.__redis.flushall()

    def __addToIndex(self, data, score):
        words = data.lower().split(' ')
        for word in words:
            word = word.strip()
            if word not in IndexEngine.IGNORE:
                if self.__redis.exists(word):
                    dbData = self.__redis.hgetall(word)
                    url = bytes(self._currentUrl, 'utf-8')
                    if url in dbData:
                        dbData[url] = bytes(str(int(dbData[url]) + score), 'utf-8')
                    else:
                        dbData[self._currentUrl] = score
                    self.__redis.hmset(word, dbData)
                else:
                    self.__redis.hmset(word, {self._currentUrl : score})

    def handle_starttag(self, tag, attrs):
        self._currentTag = tag

    def handle_data(self, data):
        if self._currentTag == 'title':
            self.__addToIndex(data, 150)
        elif self._currentTag == 'h1':
            self.__addToIndex(data, 100)
        elif self._currentTag == 'h2':
            self.__addToIndex(data, 50)
        elif self._currentTag != 'script' and self._currentTag != 'meta' and \
             self._currentTag != 'style' and self._currentTag != 'a':
            self.__addToIndex(data, 10)
