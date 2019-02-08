import tensorflow as tf
import numpy as np

sess = tf.Session()

a = tf.constant(3.0, dtype=tf.float32)
b = tf.constant(4.0)
total = tf.add( a, b )
#total = a + b
result = sess.run(total)
print( result )
#7

sess.close()