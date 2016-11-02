import redis

if __name__ == '__main__':
    redis = redis.StrictRedis()
    while True:
        results = []
        search = input('Votre recherche : ').strip()
        if search == 'quit':
            break
        dbData = redis.hgetall(search)
        for url, score in dbData.items():
            results.append((str(url, 'utf-8'), int(score)))
        results.sort(key = lambda elt : elt[1], reverse=True)
        for url, score in results:
            print('- {} ({})'.format(url, score))
        print()
