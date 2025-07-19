# Ray Tracing Math Explained Simply

## What is Ray Tracing?
Ray tracing is a technique to create realistic images by simulating how light travels and interacts with objects. Imagine shining a flashlight in a dark room—ray tracing mimics how those light rays hit objects, creating shadows, colors, and highlights.

---

## Why Do We Use Vectors?
A vector is a way to describe a direction and distance in space. In ray tracing:
- **Origin**: Where the ray starts (like your eye or a camera)
- **Direction**: Where the ray is pointing (what you’re looking at)

Vectors make it easy to describe movement, positions, and interactions in 3D.

---

## What Do Vectors Give Us?
- **Finding Intersections**: We shoot rays from the camera into the scene. Vectors help us check if a ray hits an object (like a sphere or plane).
- **Calculating Lighting**: When a ray hits something, vectors help us figure out how light bounces off the surface. This tells us how bright or dark a spot should be.
- **Shadows and Reflections**: Vectors help us see if something is in shadow (blocked from light) or if it reflects light.

---

## The Math (Easy Version)
- **Ray Equation**: A ray is defined by an origin and a direction. We use math to see where this ray meets an object.
- **Normal Vector**: When a ray hits an object, we find the “normal” (the direction sticking out from the surface). This helps us know how light should bounce.
- **Dot Product**: This is a simple math operation that tells us how much two directions line up. If the light and the normal point the same way, the spot is bright. If not, it’s darker.

---

## Visual Illustration

```
Camera (origin)
   |
   |   Ray (direction)
   v
  [Object] <--- Normal vector
      |         ^
      |         |
      |      Light direction
      v
   [Light]
```

- The camera sends out rays.
- Rays hit objects.
- At the hit point, we use the normal and light direction to calculate brightness.

---

## Why Do We Do This?
- **To Make Images Look Real**: By simulating light and how it interacts with objects, we get realistic shadows, colors, and highlights.
- **To Understand Depth**: Lighting helps us see which objects are closer or farther away.
- **To Show Materials**: Different surfaces (metal, wood, glass) react to light differently. Vectors help us show these differences.

---

## In Short
We use vectors in ray tracing because they are the best way to describe positions and directions in 3D. The math lets us figure out where rays hit, how light bounces, and how to color each pixel—making computer images look real and beautiful.

**Analogy:**
Imagine throwing lots of tiny balls (rays) from your eye into a room. Vectors help you know where each ball goes, what it hits, and how it should look when it bounces back to you!
