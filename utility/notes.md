Impossible to use scatter on the vector object.
Use scatter on vectorList and then instantiate a vector object in the destination and assign to its vectorList the array that it receives.
But need to set capacity and total
(maybe its better to loop through the sent array and add each element into the vector object)

Other possibility is to send everything, each process works on its partition of the object