import tensorflow as tf
import numpy as np

sess = tf.Session()

a = tf.placeholder(tf.int32, shape=(3, 1))
b = tf.placeholder(tf.int32, shape=(1, 3))
c = tf.matmul(a, b)

print(sess.run(c, feed_dict={a: [[3], [2], [1]], b: [[1, 2, 3]]}))
'''
[[3 6 9]
 [2 4 6]
 [1 2 3]]
'''

sess.close()