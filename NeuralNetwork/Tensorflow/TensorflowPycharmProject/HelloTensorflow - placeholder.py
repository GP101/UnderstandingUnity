import tensorflow as tf
import os

session = tf.Session()

p = tf.placeholder(tf.float32)
t = p + 1.0
#t.eval()  # This will fail, since the placeholder did not get a value.
result = session.run(t,feed_dict={p:2.0})  # This will succeed because we're feeding a value
                           # to the placeholder.
print( result )
#3.0

session.close()