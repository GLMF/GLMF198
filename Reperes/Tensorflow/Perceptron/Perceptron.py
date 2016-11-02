from random import choice
from numpy import array, dot, random

class Perceptron:

    def __init__(self, training_data, k=0.2, bias=1):
        self.bias = bias
        self.w = random.rand(len(training_data[0][0]) + 1)
        self.training_data = []
        for e in training_data:
            self.training_data.append((array(e[0] + (self.bias,)), e[1]))

        self.k = k
        self.trained = False


    def f_activation(self, x):
        if x < 0:
            return 0
        else:
            return 1


    def startTraining(self, n=100, verb=False):
        errors = []

        for i in range(n):
            e, R = choice(self.training_data)
            result = dot(self.w, e)
            delta = R - self.f_activation(result)
            errors.append(delta)
            self.w += self.k * delta * e

        self.trained = True
        if verb:
            print(errors)


    def getFunction(self):
        if self.trained:
            return lambda *e : self.f_activation(dot(array(e + (self.bias,)), self.w))
        else:
            return None


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

    perceptron = Perceptron(training_data)
    perceptron.startTraining()
    fct_xor_and = perceptron.getFunction()
    print('1 xor 0 and 1 -> {}'.format(fct_xor_and(1, 0, 1)))
