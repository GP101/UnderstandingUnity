#_20180301_jintaeks
import tensorflow
import os

session = tensorflow.Session()

x = tensorflow.constant(10)
y = tensorflow.constant(32)
z = session.run(x + y)
print( z )
#42

session.close()