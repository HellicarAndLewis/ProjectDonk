class Brownian {
    PVector pos;
    PVector vel;
    int age = 255;
    Brownian(float x, float y) {
        pos = new PVector(x, y);
        vel = new PVector(random(-1, 1), random(-3, 0));
    }
    void draw() {
        stroke(255, age);
        fill(255, age/2);
        age-= 4;
        pos.add(vel);
        float br = 0.5;
        vel.x += random(-br, br);
        vel.y += random(-br, br);// - map(age, 255, 0, 0.01, 0.1);
        float radius = map(age, 255, 0, 15, 1);
        ellipse(pos.x, pos.y, radius, radius);
    }
    
    
}
ArrayList particles = new ArrayList();

void setup() {
    frameRate(30);
    size(600, 600);
    smooth();
    noStroke();
}

void draw() {
    background(0);
    fill(255);
    for(int i = 0; i < particles.size(); i++) {
        Brownian b = (Brownian)particles.get(i);
        if(b.age<=0) {
           particles.remove(i);
           i--;
        } else {
            b.draw();
        }
    }
}

void mouseMoved() {
    for(int i = 0; i < 5; i++) particles.add(new Brownian(mouseX, mouseY));
}
