from tensorflow.examples.tutorials.mnist import input_data
import tensorflow as tf

import matplotlib.pyplot as plt
import matplotlib.cm as cm
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

prediction = tf.argmax(A, 1)
correct_prediction = tf.equal(prediction, tf.argmax(y_,1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, "float"))
print('Taux de reconnaissance : {:.2f}%'.format(sess.run(accuracy, feed_dict={e: mnist.test.images, y_: mnist.test.labels})))

index = random.randint(0, len(mnist.test.images))

img = mnist.test.images[index]
img = img.reshape((28,28))

label = mnist.test.labels[index]
num = numpy.where(label == 1)[0][0]

pred_num = sess.run(prediction, feed_dict={e: mnist.test.images, y_: mnist.test.labels})[index]

fig = plt.figure()
fig.suptitle('Écriture manuscrite d\'un {} (prédiction : {})'.format(num, pred_num), fontsize=14, fontweight='bold', color='blue')
plt.imshow(img, cmap = cm.Greys)
plt.show()
