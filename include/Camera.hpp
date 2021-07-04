//A A simple camera pplication for OpenGL
//Can be omptimized for a FPS shooter camera, see comments about jumpong below
//Not a good space-fligh camera

#ifndef CAMERA_H
#define CAMERA_H

#include <gl/glew.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

float lerp(float start, float end, float amount);

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,

    JUMP,
    CROUCH
};

const float YAW         = -90.0f;
const float PITCH       = 0.0f;
const float SPEED       = 4.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM        = 45.0f;

class Camera{
    public:
        //camera attributes
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;
        //Euler angles
        float Yaw;
        float Pitch;
        //camera options
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;

        bool jumping = false;
        bool jumpingDaccel = false;
        bool jumpWait = false; // Only should activate if user has released jump, then needs to wait until playe ris at 0.0f
        float gravity = -9.81;

        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM){
            Position = position;
            WorldUp = up;
            Yaw = yaw;
            Pitch = pitch;
            updateCameraVectors();
        }

        glm::mat4 GetViewMatrix(){
            
            return glm::lookAt(Position, Position + Front, Up);
        }

        void ProcessKeyboard(Camera_Movement direction, float deltaTime)
        {
            float velocity = MovementSpeed * deltaTime;
            if (direction == FORWARD)
                Position += Front * velocity;
            if (direction == BACKWARD)
                Position -= Front * velocity;
            if (direction == LEFT)
                Position -= Right * velocity;
            if (direction == RIGHT)
                Position += Right * velocity;
            

            /*if (direction == JUMP){
                if (Position.y >= 0.0 && Position.y <= 1.0f && jumping == false){
                    Position.y += 0.2 * velocity;
                } 
                else if (Position.y >= 1.0f){
                    jumping = true;
                }
            }

            std::cout << jumping << ", " << Position.y << std::endl;

            if (jumping == true){
                if (Position.y > 0.2f)
                    Position.y -= 0.2 * velocity;
                else
                    jumping = false;
            }*/

            if (direction == JUMP){
                jumping = true;
            }

            //if (direction != JUMP || direction != CROUCH){
                //Position.y = 0.0f;
            //}


           /* if (jumping == true){
                /*if (Position.y >= 0.0f && Position.y < 1.0f && jumpingDaccel == false)
                    Position.y += 2.0 * velocity;
                else if (Position.y > 1.0f)
                    std::cout << "going down" << std::endl;
                else if (Position.y >= 1.0f || jumpingDaccel == true)
                    jumpingDaccel = true;
                    std::cout << Position.y << std::endl;
                    if (Position.y > 0.0f)
                        Position.y -= 2.0 * velocity;*?
                    //else
                        //Position.y = 0.0f;
                        //jumpingDaccel = false;
                //std::cout << jumping << ", " << Position.y << std::endl;

                if (Position.y >= 1.0)
                    jumpingDaccel = true;
                else if (jumpingDaccel == false && Position.y >= 0.0f){
                    Position.y += 2.0 * velocity;
                }
                

            }*/


            //Position.y = 0.0f;
        }

        void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true){
            xoffset *= MouseSensitivity;
            yoffset *= MouseSensitivity;

            //Yaw += xoffset;
            Yaw = std::fmod((Yaw + xoffset), 360.0f); //Limtis Yaw movement to 360 degrees preevnts camera from turning into shacky movements along the x axis
            Pitch += yoffset;

            if (constrainPitch){
                //Ensure the lookAt vector does'nt flip on itself
                if (Pitch > 89.0)
                    Pitch = 89.0;
                if (Pitch < -89.0f)
                    Pitch = -89.0f;
            }
            //Update all vectors
            updateCameraVectors();
        }

        void ProcessMouseScroll(float yoffset){
            Zoom -= (float)yoffset;
            if (Zoom < 1.0f)
                Zoom = 1.0f;
            if (Zoom > 45.0f);
                Zoom = 45.0f;
        }
    private:
        void updateCameraVectors(){
            glm::vec3 front;
            front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            front.y = sin(glm::radians(Pitch));
            front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            Front = glm::normalize(front);
            Right = glm::normalize(glm::cross(Front, WorldUp)); // normalize the vectos, because their lengths gets closer to 0 the more you look up or down which results in slower movement.
            Up = glm::normalize(glm::cross(Right, Front));
        }
};

//utility functions
/*float lerp(float start, float end, float amount){
    return start + amount * (end - start);
}*/
#endif