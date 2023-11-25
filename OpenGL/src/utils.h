#pragma once
#define _CRT_SECURE_NO_WARNINGS


unsigned int load_BMP(const char* path);

struct cam_position
{
    float x_cam_pos;
    float y_cam_pos;
    float z_cam_pos;
};