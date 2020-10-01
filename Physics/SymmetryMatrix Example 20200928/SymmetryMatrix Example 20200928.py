import numpy as np

M = np.array([[1,2],[2,1]])
M = np.mat(M)
a = np.array([[1, -1], [1, 1]])
R = np.mat(a) #eigenvector
print("eigenvector R = ",R)
#print("eigenvector.Inverse=",R.I)

e = np.array([[3],[-1]])
e = np.mat(e)
print("eigenvalue e = ",e)

# scale test for (2,1)
v = np.array([[2],[1]])
print("scale test for v=",v)
t = R.I @ v
#print("t=",t)
t2 = np.multiply(e,t)
#print("t2=",t2)
enew = R @ t2
print("enew eigenvector = ", enew)

# test
test = np.array([[2],[1]])
test = np.mat(test)
testResult = M @ test
print("testResult = ",testResult)