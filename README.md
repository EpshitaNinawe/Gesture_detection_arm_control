🧠Core Functions of OpenCV in the System:
1.Video Capture:
 OpenCV handles real-time video streaming from the webcam, ensuring that frames are continuously supplied for analysis.
2.Frame Processing:
 Each frame is preprocessed to make interaction natural and user-friendly. This includes converting the frame into the correct color format required by the recognition pipeline and mirroring the image so that the user’s hand movements correspond intuitively to on-screen directions.
3. Visualization and Feedback:
 OpenCV is used to draw elements such as the guiding grid, gesture labels, and hand landmark points on the video feed. This provides immediate visual feedback, helping the user understand how gestures are being interpreted.
4.Spatial Mapping through Grids:
 The video frame is divided into a 3×3 grid. The position of the index finger within these regions determines directional actions such as moving left, right, forward, or backward. This grid-based approach simplifies movement recognition and minimizes misinterpretation of random hand motions.
-The screen is divided into 3*3 grid(left, center, right * top, middle, bottom)
-the position of the index finger within these zones is used to infer directional commands.
   - top -> move forward
   -bottom -> move backward
   - left -> left
   -right -> right
   - center -> Idle(None)
   - pinch by thumb and index finger -> pick/drop
  
🔍Role in the Pipeline:
MediaPipe performs the critical task of locating the hand within the camera frame and returning precise coordinates for each key point. Using these coordinates, the system can analyze finger positions and calculate distances between specific landmarks. The relative movement and distance between the thumb and index finger form the basis for recognizing “pick” and “drop” gestures. The overall position of the index finger in the visual grid then determines directional commands for robotic movement.


🧰 Installation and Setup
1. Clone the Repository
git clone https://github.com/EpshitaNinawe/Gesture_detection_arm_control.git
cd Gesture_detection_arm_control

2. Create and Activate a Virtual Environment(optional)
On Windows:
python -m venv venv
venv\Scripts\activate

3. Install Dependencies
pip install -r requirements.txt

4. Run the Project
python gesture_control.py
