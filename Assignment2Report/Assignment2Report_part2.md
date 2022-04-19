# Assignment 2 Report - Part 2:

For this assignment, I will add lights and display shaded models to complete the renderer.

## Ambient Lighting

Calculating the lighting for ambient light was my first step in the assignment.  
Here are some examples with my explanation of the results.  

### First Example

Illuminating with a white ambient light, on a blue ambient teapot, resulted in a blue teapot.  
As discussed in class, the ambient lighting is computed by multiplying the material's ambient with the light's ambient.  
Since the ambient of the teapot is blue and the ambient of the light is white, which is a 3d vector of 1's, the ambient lighting is similar to the ambient component of the teapot.  
  
![Alt Text](Images/BlueTeapotWhiteLight.png) 
  
On the other hand, illumanting with a red ambient light on the same teapot, resulted in a black teapot.  
The light's ambient red coordinate is the only non-zero coordinate, while the teapot's ambient red coordinate is the only zero coordinate.  
Multiplying these two vectors, results in a 3d vector of zeroes, which is black in color.  
  
![Alt Text](Images/BlueTeapotRedLight.png) 

### Second Example
As can be seen in the screenshot below, I changed the ambient lighting of a banana to a yellow shade, which has a non-zero value for the red, green, and blue coordinates.  
![Alt Text](Images/BananaYellowAmbient.png)  

Since all of the banana's ambient coordinates have a non-zero value, illuminating it with a red ambient light resulted in a red shaded banana.  
  
![Alt Text](Images/BananaRedLight.png)  

Illuminating with a green ambient light, resulted in a green shaded banana. 
  
![Alt Text](Images/BananaGreenLight.png)  

And illuminating with a blue ambient light resulted in a blue shaded banana.  
  
![Alt Text](Images/BananaBlueLight.png)  

## Diffuse Lighting

I then computed the diffuse component using flat shading.  
Following is a screenshot of the scene displayed in the "Ambient Lighting" first example, with a white ambient and diffuse light.  
  
![Alt Text](Images/BlueTeapotDiffuse1.png) 

### Other Teapot Diffuse Component

![Alt Text](Images/BlueTeapotDiffuse2.png) 
  
![Alt Text](Images/BlueTeapotDiffuse3.png) 

## Reflection Vector

Below are screenshots of the direction of light and reflected light in a 1080x720 window for different light positions.  
  
### Light Position 0x0
#### Direction of Light
  
![Alt Text](Images/TeapotL2_0x0.png) 
  
#### Direction of Reflected Light
  
![Alt Text](Images/TeapotR2_0x0.png) 
  
### Light Position 640x360
#### Direction of Light
  
![Alt Text](Images/TeapotL1_640x360.png) 
  
#### Direction of Reflected Light
  
![Alt Text](Images/TeapotR1_640x360.png) 
  

### Light Position 1080x720
#### Direction of Light
  
![Alt Text](Images/TeapotL3_1080x720.png) 
  
#### Direction of Reflected Light
  
![Alt Text](Images/TeapotR3_1080x720.png) 
  
## Specular Lighting

Finally, I implemented specular lighting. 
The screenshots below illustrate phong shading with and without shininess for specular lighting.  

### Without Shininess

![Alt Text](Images/PhongSpecular1.png) 
  
![Alt Text](Images/PhongSpecular2.png) 
  
![Alt Text](Images/PhongSpecular3.png) 

### With Shininess

![Alt Text](Images/PhongSpecularShininess1.png) 
  
![Alt Text](Images/PhongSpecularShininess2.png) 
  
![Alt Text](Images/PhongSpecularShininess3.png) 

## Demonstrations

To make it easier to use and demonstrate the app's functionalities and power, I implemented a "Materials" menu.  
Using the materials menu in the upper menu, the user can dress the active model with already configured materials.  
The following screenshots demonstrate some of the materials I configured on different models.  

### Plastic Banana

![Alt Text](Images/Materials/YellowPlasticBanana.png) 
  
![Alt Text](Images/Materials/CyanPlasticBanana.png) 
  
![Alt Text](Images/Materials/GreenPlasticBanana.png) 
  
![Alt Text](Images/Materials/RedPlasticBanana.png) 

### Gold, Silver and Bronze Beethoven

![Alt Text](Images/Materials/GoldBeethoven.png) 
  
![Alt Text](Images/Materials/SilverBeethoven.png) 
  
![Alt Text](Images/Materials/BronzeBeethoven.png) 

### Rubber Bunny

![Alt Text](Images/Materials/WhiteRubberBunny.png) 
  
![Alt Text](Images/Materials/YellowRubberBunny.png) 
  
![Alt Text](Images/Materials/RedRubberBunny.png) 

### More Materials

#### Emerald Teapot
![Alt Text](Images/Materials/EmeraldTeapot.png) 
  
#### Obsidian Teapot
![Alt Text](Images/Materials/ObsidianTeapot.png) 
  
#### Pearl Teapot
![Alt Text](Images/Materials/PearlTeapot.png) 
  
#### Ruby Teapot
![Alt Text](Images/Materials/RubyTeapot.png) 
  
#### Turquoise Teapot
![Alt Text](Images/Materials/TurquoiseTeapot.png) 
  
## Post-Processing Effect
  
I chose to implement Gaussian Blur as a post-processing effect.  
The screenshots below illustrate how the effect looks on different models for different kernel sizes.  
You can see the exact kernel data in the side window.  

### First Example  
![Alt Text](Images/GaussianBlur/E1Without.png) 
![Alt Text](Images/GaussianBlur/E1With.png) 
### Second Example  
![Alt Text](Images/GaussianBlur/E2Without.png) 
![Alt Text](Images/GaussianBlur/E2With.png) 
### Third Example
![Alt Text](Images/GaussianBlur/E3Without.png) 
![Alt Text](Images/GaussianBlur/E3With.png) 
