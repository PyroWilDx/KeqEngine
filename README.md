# KeqEngine

[**KeqEngine**](https://github.com/PyroWilDx/KeqEngine/) is a 2d game engine designed specifically for creating fighting games (e.g. [Hack'N Slash](https://en.wikipedia.org/wiki/Hack_and_slash)). The engine is centered around the character [Keqing](#keqing-preview), who comes with dozens of unique skills.

https://github.com/user-attachments/assets/58355315-8059-47bc-87ad-17586ba1448f

## Game Engine

### Feature List

- **Rendering** (Scene Display, Sprite Animation, Window Resizing...)
- **Camera** (Camera Lerping, Screen Shaking)
- **Entity Component System**
- **Physics** (Velocity, Gravity...)
- **Collision Detection System**
- **Particle System**
- **Attack System** (Polygon Collision, Knock-Backs...)
- **User Interface** (Text, Button, Slider, Color-Picker...)
- **Input Events** (Mouse, Keyboard)
- **Audio** (Sound Effects, Background Music)
- **Data Storage**
- **Debugging System** (HitBox Display, Frame-by-Frame Execution)

### KeqEngine UML Diagram

<img src=".readme/KeqEngineUML.png">

### Keqing Preview

https://github.com/user-attachments/assets/b05b047d-c2dc-42b4-a2f9-c0d1f239cdd1

### Game Example

Check out [```src/Main/```](src/Main/) for a complete example of how to use KeqEngine to create a game.

- **Menu**

https://github.com/user-attachments/assets/94580992-f30a-4320-92f9-43ec17304051

- **ExploSlime**

https://github.com/user-attachments/assets/87ab9ec2-87d0-4333-b0a9-7aa8abcfe14c

- **Slash'Em**

https://github.com/user-attachments/assets/5e337072-0dd2-48a6-99e1-d0b63bff6f66

## Download

<div align="center">

[<img src="https://cdn.jsdelivr.net/gh/devicons/devicon@latest/icons/windows8/windows8-original.svg" width="60"/>](https://www.mediafire.com/file/k2q9uv5jgs8djgq/KeqEngine.zip/)

</div>

## Development Set-Up

<div align="center">

[<img src="https://cdn.jsdelivr.net/gh/devicons/devicon@latest/icons/cplusplus/cplusplus-original.svg" width="60"/>](https://isocpp.org/)
&nbsp;&nbsp;&nbsp;&nbsp;
[<img src="https://cdn.jsdelivr.net/gh/devicons/devicon@latest/icons/clion/clion-original.svg" width="60"/>](https://www.jetbrains.com/clion/)
&nbsp;&nbsp;&nbsp;&nbsp;
[<img src="https://cdn.jsdelivr.net/gh/devicons/devicon@latest/icons/windows8/windows8-original.svg" width="60"/>](https://www.microsoft.com/windows/)

[SDL2](https://www.libsdl.org/) \
[Boost](https://www.boost.org/) \
[SQLite3](https://www.sqlite.org/)

</div>

### How To Use

- Initialize SQLite3.

```
cd data && sqlite3/sqlite3.exe Keqing.db < Init.sql
```

- Run w/ CLion.

---

<div align="center">
  Copyright &#169; 2024 PyroWilDx. All Rights Reserved.
</div>
