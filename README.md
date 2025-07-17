# ELEC1601
## Members: Adrian Le, Vivian, Phat Doan
# 🧭 Maze-Solving Robot with IR Sensors and Servos

This project is an Arduino-based robot that navigates and solves a maze using three IR sensor pairs and two continuous rotation servos. The robot records the time it takes to reach the end of the maze, performs a 180° spin, and returns to the starting point. It automatically stops upon recognizing it has returned.

---

## 📦 Features

- Obstacle detection using 3 IR sensor pairs (left, middle, right)
- Maze exploration with turning and dead-end detection logic
- Turn counting and time tracking for return navigation
- Autonomous return and stop mechanism
- Visual LED indicators for system state

---

## 🛠️ Hardware Connections

| Component         | Arduino Pin(s) |
|-------------------|----------------|
| Left IR LED       | 10             |
| Left IR Receiver  | 11             |
| Right IR LED      | 2              |
| Right IR Receiver | 3              |
| Middle IR LED     | 6              |
| Middle IR Receiver| 7              |
| Left Red LED      | A2             |
| Right Red LED     | A0             |
| Middle Red LED    | A1             |
| Left Servo        | 13             |
| Right Servo       | 12             |

---

## ⚙️ Function Overview

### 🔄 Motion Control

| Function           | Description                              |
|--------------------|------------------------------------------|
| `forward(time)`     | Move forward for a given time           |
| `backward(time)`    | Move backward                          |
| `turn_r(time)`      | Rotate robot to the right              |
| `turn_l(time)`      | Rotate robot to the left               |
| `stand(time)`       | Stop robot (neutral signal)            |
| `stop()`            | Fully stop and detach servos           |
| `modify_l(time)`    | Slight left adjust to reorient         |
| `modify_r(time)`    | Slight right adjust to reorient        |

### 🧠 Maze Logic

| Function             | Description                                             |
|----------------------|---------------------------------------------------------|
| `finish()`           | Called at the end of the maze; initiates return logic  |
| `spin()`             | Rotates robot to face the way it came from             |
| `check_finish(flag)` | Checks if robot has returned to start and should stop  |

### 👀 IR Sensing

| Function                         | Description                                      |
|----------------------------------|--------------------------------------------------|
| `irDistance(ledPin, recvPin)`    | Measures distance using frequency sweep         |
| `irDetect(ledPin, recvPin, freq)`| Reads IR signal at specific frequency           |

---

## 🔍 Main Logic (loop)

- Reads 3 IR sensors for proximity
- Based on sensor values:
  - Detects walls and paths
  - Chooses to go forward, turn, or backtrack
  - Calls `finish()` when dead-end is reached
- Tracks number of turns and time of last turn
- Uses `check_finish()` during return journey to stop at the start

---

## 🧾 Mode Logic

- `mode == 0`: Exploring maze
- `mode == 1`: Returning to start
- `finish_flag == 1`: Indicates that maze is finished and robot is returning
- Robot stops when:
  - Enough time has passed since last turn
  - Sufficient number of turns have been made

---

## 📊 Sensor Values

- `irDetect()` returns:
  - `0`: Obstacle detected (IR reflected)
  - `1`: No detection (open path)
- `irDistance()` returns sum of multiple frequency checks:
  - Lower value = closer object
  - Used to determine wall proximity

---

## 📷 LED Indicators

- `redLedPin_mid` ON: Too many turns (possibly lost or looping)
- `redLedPin_left` + `redLedPin_right` ON: Maze end detected
- `redLedPin_mid` OFF: Start of return journey

---

## 📂 File

- `main.ino` – Complete Arduino sketch

---

## 📌 To-Do Ideas

- Add encoder feedback for precise turns
- Store turn sequence for perfect return path
- Add start/stop button with debounce logic
- Use more robust IR frequency handling

---

## 💬 License

MIT License – feel free to use, modify, and share with attribution.
