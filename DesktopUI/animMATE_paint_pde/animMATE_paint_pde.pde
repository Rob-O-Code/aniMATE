import java.util.*;
//import java.lang.*;
//class aniMate{
//InputStreamReader read = new InputStreamReader(System.in);
//BufferedReader in = new BufferedReader(read);
//ArrayList<Square> particles = new ArrayList<Square>();
float r=0;
float g=0;
float b=0;
int penSize = 1;

void setup() {
  /*Scanner in = new Scanner(System.in);
   println("Enter the number of frames");
   int frames = in.nextInt();
   println("Enter the number of lights in each strip: ");
   int lights = in.nextInt();*/
  size(800, 650);
  background(255);
}

final int numLEDs = 6;
final int numFrames = 8;

color rectangles[][] = new color[numFrames][numLEDs];
void draw() {
  int frame = millis() / 200 % numFrames;
  for (int col = 0; col < numLEDs; col++) {
    fill(rectangles[frame][col]);
    circle(700, 50+60*col, 25);
  }
  stroke(r, g, b);
  strokeWeight(penSize);
  if (mousePressed&&mouseY<height-50) {
    line(pmouseX, pmouseY, mouseX, mouseY);
  }
  strokeWeight(3);
  line(0, height-50, width, height-50);
  stroke(0);
  strokeWeight(1);
  fill(255, 0, 0);
  rect(0, height-50, 50, 50);
  fill(0, 255, 0);
  rect(50, height-50, 50, 50);
  fill(0, 0, 255);
  rect(100, height-50, 50, 50);
  fill(255);
  rect(150, height-50, 50, 50);
  fill(0);
  rect(200, height-50, 50, 50);
  fill(255);
  rect(250, height-50, 50, 50);
  fill(0);
  text("Random", 255, height-25);
  
  for (int row = 0; row < numFrames; row++) {
    for (int col = 0; col < numLEDs; col++) {
      if (mousePressed && mouseX < col*100+50 && mouseX > col*100 && mouseY > row*70 && mouseY < row*70+50) {
        rectangles[row][col] = color(r,g,b);
      }
      fill(rectangles[row][col]);
      rect(col*100, row*70, 50, 50);
    }
  }
  
  /*
  for (int j=1, k=1; j<=5; j++, k++) {
    if (mousePressed&&mouseX<j*100&&mouseY<j+1) {
      rectangles[j][k]=get(mouseX, mouseY);
      fill(get(mouseX, mouseY));
      rect(j*100, (j+1), 50, 50);
    } else {
      fill(255);
      rect(j*100, (j+1), 50, 50);
    }

    fill(255);
    rect(k*100, 80, 50, 50);
    fill(255);
    rect(k*100, 150, 50, 50);
    fill(255);
    rect(k*100, 225, 50, 50);
    fill(255);
    rect(k*100, 300, 50, 50);
  }*/
  if (mousePressed&&mouseX<50&&mouseY<height-50) {
    r=255;
    g=0;
    b=0;
  }
  if (mousePressed&&mouseX>50&&mouseX<100&&mouseY>height-50) {
    r=0;
    g=255;
    b=0;
  }
  if (mousePressed&&mouseX>100&&mouseX<150&&mouseY>height-50) {
    r=0;
    g=0;
    b=255;
  }
  if (mousePressed&&mouseX>150&&mouseX<200&&mouseY>height-50) {
    r=255;
    g=255;
    b=255;
  }
  if (mousePressed&&mouseX>200&&mouseX<250&&mouseY>height-50) {
    r=0;
    g=0;
    b=0;
  }
  if (mousePressed&&mouseX>250&&mouseX<300&&mouseY>height-50) {
    r=random(255);
    g=random(255);
    b=random(255);
  }

  /*for(int i=0;i<50;i*=10)
   {
   for(int j=0;j<5;j++)
   {
   rect(i, height-10, 10, 10);
   }
   }*/

  //pen size buttons

  fill(0);
  if (mousePressed&&mouseX>350-10&&mouseX<350+10&&mouseY>height-50) {
    penSize=50;
    fill(255);
  }
  ellipse(350, height-25, 50, 50);
  fill(0);
  if (mousePressed&&mouseX>400-10&&mouseX<400+10&&mouseY>height-50) {
    penSize=25;
    fill(255);
  }
  ellipse(400, height-25, 25, 25);
  fill(0);
  if (mousePressed&&mouseX>430-10&&mouseX<430+10&&mouseY>height-50) {
    penSize=10;
    fill(255);
  }
  ellipse(430, height-25, 10, 10);
  fill(0);
  if (mousePressed&&mouseX>450-10&&mouseX<450+10&&mouseY>height-50) {
    penSize=3;
    fill(255);
  }
  ellipse(450, height-25, 3, 3);

  // Save Button
  fill(255);
  rect(500, height-50, 50, 50);
  fill(0);
  text("Save", 510, height-25);

  // Erase Button
  fill(100);
  rect(550, height-50, 250, 50);
  fill(0);
  text("Erase", 670, height-25);
}
void mousePressed() {
  if (mouseX>500&&mouseX<550&&mouseY>height-50) {
    PGraphics pg = createGraphics(numLEDs, numFrames);
    pg.beginDraw();
    pg.loadPixels();
    for (int row = 0; row < numFrames; row++) {
      for (int col = 0; col < numLEDs; col++) {
        pg.pixels[row*numLEDs + col] = rectangles[row][col];
      }
    }
    pg.updatePixels();
    pg.endDraw();
    pg.save("f01.bmp");
  }
  if (mouseX>550&&mouseX<800&&mouseY>height-50) {
    background(255);
  }
}
