# physics-trussPhysics
This is a work in progress. Work with soft body physics using the Method of Joints and Finite Element Method.

The general idea is to take the object and treat it as a 2D planar truss.
Then use the method of joints and solve for the Support Forces and then the Bar Forces.
Once we have solved for this, use the stiffness matrix to figure out the nodal displacement.
Finally, now that we have the nodal displacement, using interpolation we will make the nodes mode.

The above was a brief overview of the process.
