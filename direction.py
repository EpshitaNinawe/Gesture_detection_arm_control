import cv2
import mediapipe as mp
import math
import time

# Initialize Mediapipe
mp_hands = mp.solutions.hands
hands = mp_hands.Hands(max_num_hands=1, min_detection_confidence=0.7)
mp_draw = mp.solutions.drawing_utils

# Open webcam
cap = cv2.VideoCapture(0)
cap.set(3, 640)
cap.set(4, 480)

# Timing and gesture state
last_action_time = time.time()
cooldown = 0.2
picked = False
prev_pinch = False
last_action = "None"

def calculate_distance(p1, p2):
    return math.hypot(p2[0] - p1[0], p2[1] - p1[1])

while True:
    success, img = cap.read()
    if not success:
        break

    img = cv2.flip(img, 1)
    img_rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    results = hands.process(img_rgb)
    h, w, _ = img.shape

    current_action = last_action

    if results.multi_hand_landmarks:
        for handLms in results.multi_hand_landmarks:
            mp_draw.draw_landmarks(img, handLms, mp_hands.HAND_CONNECTIONS)

            # Key landmarks
            index_tip = handLms.landmark[mp_hands.HandLandmark.INDEX_FINGER_TIP]
            thumb_tip = handLms.landmark[mp_hands.HandLandmark.THUMB_TIP]

            x, y = int(index_tip.x * w), int(index_tip.y * h)
            thumb_x, thumb_y = int(thumb_tip.x * w), int(thumb_tip.y * h)

            cv2.circle(img, (x, y), 10, (0, 255, 0), cv2.FILLED)
            cv2.circle(img, (thumb_x, thumb_y), 10, (255, 0, 0), cv2.FILLED)

            # Pick/Drop detection
            dist = calculate_distance((x, y), (thumb_x, thumb_y))
            pinch = dist < 40

            current_time = time.time()
            if current_time - last_action_time > cooldown:
                # Grid boundaries
                left_limit = w // 3
                right_limit = 2 * w // 3
                top_limit = h // 3
                bottom_limit = 2 * h // 3

                # --- PICK / DROP ---
                if pinch and not prev_pinch:
                    if not picked:
                        current_action = "Pick"
                        picked = True
                    else:
                        current_action = "Drop"
                        picked = False

                # --- MOVEMENT (only index finger, not pinching) ---
                elif not pinch:
                    if x < left_limit and top_limit < y < bottom_limit:
                        current_action = "Move Left"
                    elif x > right_limit and top_limit < y < bottom_limit:
                        current_action = "Move Right"
                    elif y < top_limit and left_limit < x < right_limit:
                        current_action = "Move Forward"
                    elif y > bottom_limit and left_limit < x < right_limit:
                        current_action = "Move Backward"
                    elif left_limit < x < right_limit and top_limit < y < bottom_limit:
                        # center zone (no movement)
                        if "Pick" in last_action or "Drop" in last_action:
                            current_action = last_action
                        else:
                            current_action = "None"

                prev_pinch = pinch
                last_action_time = current_time

    else:
        if "Move" in last_action:
            current_action = last_action
        else:
            current_action = "None"

    last_action = current_action

    # Draw grid
    cv2.line(img, (w // 3, 0), (w // 3, h), (255, 255, 255), 2)
    cv2.line(img, (2 * w // 3, 0), (2 * w // 3, h), (255, 255, 255), 2)
    cv2.line(img, (0, h // 3), (w, h // 3), (255, 255, 255), 2)
    cv2.line(img, (0, 2 * h // 3), (w, 2 * h // 3), (255, 255, 255), 2)

    # Display current action
    cv2.putText(img, f'Action: {current_action}', (10, 40),
                cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 255), 2)

    cv2.imshow("Gesture Control Grid System", img)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
