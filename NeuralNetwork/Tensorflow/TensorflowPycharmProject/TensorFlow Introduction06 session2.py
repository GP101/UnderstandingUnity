import tensorflow as tf
import numpy as np

sess = tf.Session()

vec = tf.random_uniform(shape=(3,))
out1 = vec + 1
out2 = vec + 2

print(sess.run((out1,out2)))

sess.close()