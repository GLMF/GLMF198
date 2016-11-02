from tensorflow.examples.tutorials.mnist import input_data
import tensorflow as tf

import matplotlib.pyplot as plt
import matplotlib.cm as cm
import matplotlib.pylab as pylab
import matplotlib
import random
import numpy

e = tf.placeholder(tf.float32, [None, 784]) 
w = tf.Variable(tf.zeros([784 , 10])) 
bias = tf.Variable(tf.zeros([10]))

A = tf.nn.softmax(tf.matmul(e, w) + bias) 

y_ = tf.placeholder(tf.float32, [None, 10 ])
cross_entropy = tf.reduce_mean(-tf.reduce_sum(y_ * tf.log(A), reduction_indices=[1]))

train_step = tf.train.GradientDescentOptimizer(0.2).minimize(cross_entropy) 

init = tf.initialize_all_variables()

sess = tf.Session()
sess.run(init)

mnist = input_data.read_data_sets("MNIST_data/", one_hot=True)

for i in range(1000):
    batch_xs, batch_ys = mnist.train.next_batch(100)
    sess.run(train_step, feed_dict={e: batch_xs, y_: batch_ys})

colormap_dict = {
    'red':   [(0.0,  1.0, 1.0),
              (0.0, 1.0, 1.0),
              (1.0,  0.25, 0.0),
              (1.0,  0.0, 0.0)
             ],
    'blue':  [(0.0,  0.0, 0.0),
              (1.0,  0.0, 0.0)
             ],
    'green': [(0.0,  0.0, 0.0),
              (0.5,  0.0, 0.0),
              (0.75, 1.0, 1.0),
              (1.0,  1.0, 1.0)
             ]
}
redblue = matplotlib.colors.LinearSegmentedColormap('red_black_blue', colormap_dict, 256)

wts = w.eval(sess)

for i in range(0,10):
    im = wts.flatten()[i::10].reshape((28,-1))
    plt.subplot(3, 4, i + 1)
    plt.imshow(im, cmap = redblue, clim=(-1.0, 1.0))
    plt.title('Chiffre {}'.format(i), fontsize=14, fontweight='bold', color='blue')
    plt.colorbar()

plt.suptitle('Impact des poids', fontsize=14, fontweight='bold', color='blue')
pylab.gcf().canvas.set_window_title('Analyse des poids après apprentissage')
plt.show()


