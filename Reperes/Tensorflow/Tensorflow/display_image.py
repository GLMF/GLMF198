from tensorflow.examples.tutorials.mnist import input_data
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import matplotlib
import random
import numpy

mnist = input_data.read_data_sets("MNIST_data/", one_hot=True)
index = random.randint(0, len(mnist.train.images))

img = mnist.train.images[index]
img = img.reshape((28,28))

label = mnist.train.labels[index]
num = numpy.where(label == 1)[0][0]

fig = plt.figure()
fig.suptitle('Écriture manuscrite d\'un {}'.format(num), fontsize=14, fontweight='bold', color='blue')
plt.imshow(img, cmap = cm.Greys)
plt.show()
