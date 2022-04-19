# Assignment 2 Report - Part 1:

The purpose of this assignment is to paint full triangles and implement the z-buffer algorithm.  

## Drawing Bounding Rectangles
I began by implementing a feature to draw the bounding rectangle for each triangle in a different color.  
The feature can be toggled on and off.  

![Alt Text](Images/BoundingRectangles.png) 

## Filling Triangles
I then changed the DrawTriangle function to fill each triangle with a random color, using the edge walking approach.  

![Alt Text](Images/FillTriangles.png) 

## The Z-Buffer Algorithm
In order to overcome triangles overlapping, I implemented the z-buffer algorithm.  
The following screenshots compare the color buffer and z-buffer.  

### Bunny using Color Buffer
![Alt Text](Images/BunnyColorBuffer_2.png)
### Bunny using Z-Buffer
![Alt Text](Images/BunnyZBuffer_2.png) 
### Teapot using Color Buffer
![Alt Text](Images/TeapotColorBuffer.png)
### Teapot using Z-Buffer
![Alt Text](Images/TeapotZBuffer.png) 
### Bishop using Color Buffer
![Alt Text](Images/BishopColorBuffer.png)
### Bishop using Z-Buffer
![Alt Text](Images/BishopZBuffer.png) 
