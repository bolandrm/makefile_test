#ifndef IMU_H
#define IMU_H

#include <stdint.h>

#define GYRO_X_OFFSET (-68.94)
#define GYRO_Y_OFFSET (-46.81)
#define GYRO_Z_OFFSET 54.23

#define ACCEL_X_OFFSET (-30.40)
#define ACCEL_Y_OFFSET (-91.63)
#define ACCEL_Z_OFFSET (-279.43)

#define GYRO_PART 0.994
#define ACC_PART (1.0 - GYRO_PART)

typedef struct {
  int16_t x, y, z;
} axis_int16_t;

typedef struct {
  int16_t x, y, z;
} axis_int32_t;

typedef struct {
  float x, y, z;
} axis_float_t;

void imu_init();
void imu_read_raw_values();
void imu_process_values();

axis_float_t imu_rates();
axis_float_t imu_angles();
axis_float_t imu_gyro_angles();
axis_int16_t imu_gyro_raws();
axis_int16_t imu_accel_raws();
axis_float_t imu_accel_filtered();
axis_float_t imu_accel_angles();
uint32_t imu_value_process_dt();

#endif
