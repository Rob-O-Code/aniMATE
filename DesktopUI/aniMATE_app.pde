/**
 * Load and Display 
 * 
 * Images can be loaded and displayed to the screen at their actual size
 * or any other size. 
 */

PImage img;  // Declare variable "a" of type PImage

void setup() {
  size(640, 360);
  // The image file must be in the data folder of the current sketch 
  // to load successfully
  img = loadImage("TestImg.bmp");  // Load the image into the program
  final int size = 10;
  for (int x = 0; x < img.width; x++) {
    for (int y = 0; y < img.height; y++) {
      color c = img.get(x, y);
      fill(c);
      noStroke();
      rect(x*size+100, y*size+100, size, size);
    }
  }
}

// Color picker/palette
// Display current color somewhere on screen
//   Should be a square on the screen that always shows the current color
// Change the color of a single pixel in the grid
//
// Advanced:
//   Different size brushes
//   Draw lines
//   Fill tool
//   Draw shapes
//
// Finally:
//   Output image to .bmp
//   Demonstrate the animation (time-based for now)
//   Configure properties (separate .txt with size, animation type, etc.)

color currentColor = color(0);
void draw() {
  // Draw color palette
  fill(255, 0, 0); // R, G, B
  noStroke();
  rect(10, 10, 50, 50);
  fill(255, 255, 0);
  rect(60, 10, 50, 50);
  
  // Color palette code
  if (mousePressed) {
    // Check if we are inside the palette
    if (false) { // mouseX and mouseY are variables
      updatePixels();
      // Set currentColor to pixels[x+y*width]
      loadPixels();
    }
  }
  
  
}
