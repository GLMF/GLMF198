class Faux_Perceptron:

    def __init__(self, training_data):
        self.training_data = {}
        for e in training_data:
            self.training_data[e[0]] = e[1]


    def getFunction(self):
        return lambda *e : self.training_data[e]


if __name__ == '__main__':
    training_data = (
        ((0, 0, 0), 0),
        ((0, 0, 1), 0),
        ((0, 1, 0), 0),
        ((0, 1, 1), 1),
        ((1, 0, 0), 0),
        ((1, 0, 1), 1),
        ((1, 1, 0), 0),
        ((1, 1, 1), 0)
    )

    f_perceptron = Faux_Perceptron(training_data)
    fct_xor_and = f_perceptron.getFunction()
    print('1 xor 1 and 1 -> {}'.format(fct_xor_and(1, 1, 1)))
