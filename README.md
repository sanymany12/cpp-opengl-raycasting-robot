# Interactive 3D Robot Simulation

An interactive 3D computer graphics application built from scratch using **C++** and **modern OpenGL**. The project demonstrates hierarchical 3D transformations, complex lighting models, camera manipulation, and mouse-based raycasting.

---

## 🚀 Key Features

*   **Hierarchical Model Transformations:** Implemented character anatomy where the movement and rotation of child joints (e.g., lower arms, legs, head) are hierarchically linked to the parent transform (torso) using matrix multiplications.
*   **Mouse Picking & Raycasting:** Translates 2D screen coordinates from mouse clicks into 3D world coordinates. A custom ray-plane intersection algorithm calculates the exact target position on the table surface for robot navigation.
*   **Autonomous Robot Movement:** A state-machine-driven logic handles smooth transition between standing, rotating towards the target, and moving forward with automated walking animations.
*   **Advanced Lighting & Texturing:** Includes Blinn-Phong shading with multi-light support (ambient, diffuse, specular), attenuation, and texture mapping (including diffuse and specular/shine maps).
*   **Real-time GUI Control:** Integrated **ImGui** to allow live configuration of joint angles, movement speeds, shader debug modes (viewing raw normals, diffuse maps, etc.), and camera switching.
*   **Dynamic Cameras:** Features a flexible first/third-person manipulator camera and a dedicated 3rd person tracking camera attached to the robot's local space.

---

## 🛠️ Tech Stack & Libraries

*   **Language:** C++ (Modern standards, clean memory management)
*   **Graphics API:** OpenGL (Core Profile) & GLSL for Shaders
*   **Window & Input Management:** SDL3
*   **Extension Loader:** GLEW
*   **GUI:** Dear ImGui (with SDL3 + OpenGL3 binding)
*   **Mathematics:** GLM (OpenGL Mathematics)

---

## 📸 Media / Showcase
![Project Demo](robot_showcase.gif)

---

## ⚙️ How to Build and Run

### Prerequisites
Make sure you have the following libraries configured/installed on your system:
*   SDL3
*   GLEW
*   GLM
*   Dear ImGui
